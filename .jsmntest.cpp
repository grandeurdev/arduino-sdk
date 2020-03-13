#include "jsmn/jsmn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
    "{\"device\": {\"deviceID\": \"ck1257\", productID: \"FRI-1\"}, \"admin\": {\"hello\": 1, \"Bye\": 50, \"hello\": {\"eee\": \"yayy\"}}, \"uid\": 1000, \"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

int main() {
  jsmn_parser parser;
  jsmntok_t tokens[128]; /* We expect no more than 128 tokens */

  jsmn_init(&parser);
  int r = jsmn_parse(&parser, JSON_STRING, strlen(JSON_STRING), tokens,
                 sizeof(tokens) / sizeof(tokens[0]));
  if (r < 0) {
    printf("Failed to parse JSON: %d\n", r);
    return 1;
  }

  /* Assume the top-level element is an object */
  if (r < 1 || tokens[0].type != JSMN_OBJECT) {
    printf("Object expected\n");
    return 1;
  }

  /* Loop over all keys of the root object */
  for (int i = 1; i < r; i++) {
    if (tokens[i + 1].type == JSMN_OBJECT) {
      for (int j = 1; j < tokens[i + 1].size*2; j+=2) {
          if (tokens[i + j + 2].type == JSMN_OBJECT) {
              break;
          }
        printf("- %.*s: %.*s\n", tokens[i + j + 1].end - tokens[i + j + 1].start, JSON_STRING + tokens[i + j + 1].start, tokens[i + j + 2].end - tokens[i + j + 2].start, JSON_STRING + tokens[i + j + 2].start);
      }
      i += tokens[i + 1].size*2 + 1;
      /* We may additionally check if the value is either "true" or "false" */
    }
    else if (tokens[i + 1].type == JSMN_STRING) {
      /* We may use strndup() to fetch string value */
      printf("- %.*s: %.*s\n", tokens[i].end - tokens[i].start,
             JSON_STRING + tokens[i].start, tokens[i + 1].end - tokens[i + 1].start,
             JSON_STRING + tokens[i + 1].start);
      i++;
    } else if (tokens[i + 1].type == JSMN_PRIMITIVE) {
      /* We may want to do strtol() here to get numeric value */
      printf("- %.*s: %.*s\n", tokens[i].end - tokens[i].start,
             JSON_STRING + tokens[i].start, tokens[i + 1].end - tokens[i + 1].start,
             JSON_STRING + tokens[i + 1].start);
      i++;
    } else if (tokens[i + 1].type == JSMN_ARRAY) {
      int j;
      for (j = 0; j < tokens[i + 1].size; j++) {
        jsmntok_t *g = &tokens[i + j + 2];
        printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
      }
      i += tokens[i + 1].size + 1;
    } else {
      printf("Unexpected JSON type: %.*s\n", tokens[i].end - tokens[i].start,
             JSON_STRING + tokens[i].start);
    }
  }
  return EXIT_SUCCESS;
}
