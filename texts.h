#ifndef TEXTS_H
#define TEXTS_H

void set_keypress(void);

void reset_keypress(void);

int texts_end(wchar_t* str);

int texts_get_count();

wchar_t** texts_get(unsigned int text_src);

void texts_printf(wchar_t** text);

void texts_read(wchar_t** text);

#endif
