/*
 * Copyright (c) 2013 Martin Donath, voola GmbH <martin.donath@voola.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

/* ----------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------- */

static struct config {
  struct {
    char **file;                       /* Input files */
    size_t size;                       /* Input file count */
  } input;
  struct {
    int32_t sock;                      /* Server socket handle */
    int32_t port;                      /* Server listen port */
  } server;
  uint8_t loop;                        /* Play an infinite loop */
  size_t rate;                         /* Playback rate (default: 100) */
} config = {
  .rate = 100
};

/* ----------------------------------------------------------------------------
 * Internal functions
 * ------------------------------------------------------------------------- */

/*!
 * Print usage information to standard error and exit.
 */
static void
usage() {
  fprintf(stderr,
    "Usage: play [options] port [file ...]\n"
    "\n"
    "Options:\n"
    "  -h, --help                 print this message and exit\n"
    "  -l, --loop                 play an infinite loop\n"
    "  -r, --rate                 playback rate (default: 100)\n");
  exit(EXIT_FAILURE);
}

/*!
 * Parse command-line options.
 *
 * \param[in] argc   Argument count
 * \param[in] argv[] Arguments
 */
static void
parse(int argc, char *argv[]) {
  int arg;

  /* Specify available options */
  struct option options[] = {
    { "help",      no_argument,       0, 'h' },
    { "loop",      no_argument,       0, 'l' },
    { "rate",      required_argument, 0, 'r' }
  };

  /* Iterate and parse options */
  while ((arg = getopt_long(argc, argv, "hlr:", options, NULL)) != -1)
    switch (arg) {

      /* -h: print usage information and exit */
      case 'h':
      case '?':
        usage();

      /* -l: play an infinite loop */
      case 'l':
        config.loop = 1;
        break;

      /* -r: playback rate */
      case 'r':
        if (!(config.rate = atoi(optarg)))
          fprintf(stderr, "play: invalid playback rate\n"),
          usage();
        break;

      /* You gotta catch 'em all */
      default:
        abort();
    }

  /* Check for port */
  if (optind == argc || !(config.server.port = atoi(argv[optind++])))
    fprintf(stderr, "play: invalid port\n"),
    usage();

  /* Check for at least one input file */
  if (optind == argc)
    fprintf(stderr, "play: no input file(s)\n"),
    usage();

  /* Map input files */
  config.input.file = &(argv[optind]);
  config.input.size = argc - optind;
}

/* ----------------------------------------------------------------------------
 * Program
 * ------------------------------------------------------------------------- */

/*!
 * Open a TCP server on the specified port, read a list of files and send them
 * line-by-line one after another to any connected client.
 *
 * \param[in] argc   Argument count
 * \param[in] argv[] Arguments
 * \return           Exit code
 */
int
main(int argc, char *argv[]) {
  parse(argc, argv);

  /* Create a new TCP socket */
  int32_t flag = 1;
  if ((config.server.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    fprintf(stderr, "play: could not create socket -- %s\n", strerror(errno)),
    usage();

  /* Set socket options and initialize address struct */
  setsockopt(config.server.sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  setsockopt(config.server.sock, SOL_SOCKET, SO_NOSIGPIPE, &flag, sizeof(flag));

  /* Set address family, port and bind address */
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port   = htons(config.server.port),
    .sin_addr   = {
      .s_addr   = htonl(INADDR_LOOPBACK)
    }
  };

  /* Bind the socket and put it into listening mode */
  if (bind(config.server.sock, (void *)&addr, sizeof(addr)) < 0)
    fprintf(stderr, "play: could not bind socket -- %s\n", strerror(errno)),
    usage();
  else if (listen(config.server.sock, 5))
    fprintf(stderr, "play: could not listen on socket -- %s\n", strerror(errno)),
    usage();

  /* Accept loop */
  do {
    struct sockaddr_in client; socklen_t client_size = sizeof(client);
    int32_t conn = accept(config.server.sock,
      (struct sockaddr *)&client, &client_size);

    /* Fork and process connection */
    int pid = fork();
    if (pid == 0) {
      close(config.server.sock);

      /* Iterate input files */
      uint8_t error = 0;
      do {
        for (size_t f = 0; !error && f < config.input.size; f++) {
          FILE *file = fopen(config.input.file[f], "r");
          if (!file)
            fprintf(stderr, "play: could not open file %s -- %s\n",
              config.input.file[f], strerror(errno)),
            usage();

          /* Now read and send input files line-by-line via socket */
          char line[4096];
          while (!error && fgets(line, sizeof(line), file)) {
            if (write(conn, line, strlen(line)) == -1)
              error = 1;
            if (config.rate)
              usleep(1000000 / config.rate);
          }
          fclose(file);
          if (error)
            break;
        }
      } while (!error && config.loop);

      /* Close connection and exit */
      close(conn);
      exit(EXIT_SUCCESS);
    }

    /* Prevent zombie child processes */
    close(conn); wait(NULL);
  } while (1);
  return 0;
}