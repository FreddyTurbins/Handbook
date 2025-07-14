#define GAP_BUFFER_IMPLEMENTATION
#include "gap_buffer.h"

signed main()
{
  GapBuffer gap_buffer = gap_buffer_create(1);
  gap_buffer_insert(&gap_buffer, "ho", 0);
  gap_buffer_insert(&gap_buffer, "ell", 1);
  //printf("EY-> %s\n", gap_buffer.data + gap_buffer.gap_end);
  gap_buffer_insert(&gap_buffer, "world!", 5);
  gap_buffer_insert(&gap_buffer, " my ", 5);
  //printf("%s\n", gap_buffer.data)
  char* str = (char*)malloc(30);
  memcpy(str, gap_buffer.data, gap_buffer.gap_start);
  memcpy(str + gap_buffer.gap_start, gap_buffer.data + gap_buffer.gap_end, gap_buffer.buffer_size - gap_buffer.gap_end);
  printf("%s\n", str);
  free(str);
  
  gap_buffer_delete(&gap_buffer, 5, 3);

  str = (char*)malloc(30);
  memcpy(str, gap_buffer.data, gap_buffer.gap_start);
  memcpy(str + gap_buffer.gap_start, gap_buffer.data + gap_buffer.gap_end, gap_buffer.buffer_size - gap_buffer.gap_end);
  printf("%s\n", str);
  free(str);
  free(gap_buffer.data);
}
