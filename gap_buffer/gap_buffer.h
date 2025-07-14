/*************************************************************************************************
*
*     Gap buffer abstraction
*
*************************************************************************************************/
#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#ifdef _WIN32
  #define GBAPI __declspec(dllexport)
#endif

#ifndef GBAPI
  #define GBAPI
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct GapBuffer {
  size_t          gap_end;
  size_t          gap_start;
  size_t          gap_count;
  size_t          buffer_size;

  char*           data;
} GapBuffer;

GBAPI GapBuffer gap_buffer_create(const size_t gap_size);
GBAPI void gap_buffer_free(GapBuffer* gap_buffer);
GBAPI void gap_buffer_insert(GapBuffer* gap_buffer, const char* str, const size_t position);
GBAPI void gap_buffer_delete(GapBuffer* gap_buffer, const size_t position, const size_t bytes);

#if defined(GAP_BUFFER_IMPLEMENTATION)

GBAPI static void _gap_buffer_shift_gap_position(GapBuffer* gap_buffer, const size_t position);

#define ft_assert(condition, message)                 \
  if (condition) {                                    \
    perror(message);                                  \
    abort();                                          \
  }

GapBuffer gap_buffer_create(const size_t gap_size)
{
  GapBuffer gap_buffer = {
    .gap_end        = gap_size-1,
    .gap_start      = 0,
    .gap_count      = 0,
    .buffer_size    = gap_size,
    .data           = (char*)calloc(gap_size, sizeof(int8_t))
  };
  ft_assert(gap_buffer.data == NULL, 
      "create_gap_buffer -> Allocate error, data in gap not allocated\n");
  return gap_buffer;
}

void gap_buffer_free(GapBuffer* gap_buffer)
{
  if (gap_buffer->data == NULL) return;
  free(gap_buffer->data);
  gap_buffer->gap_end = 0;
  gap_buffer->gap_start = 0;
  gap_buffer->gap_count = 0;
  gap_buffer->buffer_size = 0;
}

void gap_buffer_insert(GapBuffer* gap_buffer, const char* str, const size_t position)
{
  const size_t str_size = strlen(str);
  ft_assert(position > gap_buffer->gap_count, 
      "gap_buffer_insert -> Position greater than string_count");

  while (gap_buffer->gap_count + 2 * (str_size+1) >= gap_buffer->buffer_size) {
    _gap_buffer_shift_gap_position(gap_buffer, gap_buffer->gap_count);
    gap_buffer->buffer_size <<= 1;
    gap_buffer->data = (char*)realloc(gap_buffer->data, gap_buffer->buffer_size * sizeof(char));
    ft_assert(gap_buffer->data == NULL,
        "gap_buffer_insert -> Realloc error, buy more ram");
    memset(gap_buffer->data + gap_buffer->gap_end, 0, gap_buffer->buffer_size - gap_buffer->gap_end);
    gap_buffer->gap_end = gap_buffer->buffer_size-1;
  }
  _gap_buffer_shift_gap_position(gap_buffer, position);
  memcpy(gap_buffer->data + gap_buffer->gap_start, str, str_size);

  gap_buffer->gap_start += str_size;
  gap_buffer->gap_count += str_size;
}

void gap_buffer_delete(GapBuffer* gap_buffer, const size_t position, const size_t bytes)
{
  const size_t total_bytes = gap_buffer->buffer_size - position;
  ft_assert(total_bytes < bytes, 
      "gap_buffer_delete -> Invalid size\n");

  _gap_buffer_shift_gap_position(gap_buffer, position);
  gap_buffer->gap_end   += bytes;
  gap_buffer->gap_count -= bytes;
}

static void _gap_buffer_shift_gap_position(GapBuffer* buffer, const size_t position)
{
  if (position == buffer->gap_start) return;

  const size_t delta = (buffer->gap_start - position) * sizeof(char);
  if (position < buffer->gap_start) {
    memmove(buffer->data + buffer->gap_end - delta, buffer->data + position, delta);
    buffer->gap_end -= delta;
  } else {
    memmove(buffer->data + buffer->gap_start, buffer->data + buffer->gap_end, -delta);
    buffer->gap_end += delta;
  }
  
  buffer->gap_start = position;
}
#endif
#endif
