/* iter.h
 * #Implementation#
 * Functions, type definitions and macro constants in C99 — C2x
 */
#include <stdio.h>
typedef FILE type-name; // each FILE object denotes a C stream.
char *tempnam(const char *dir, const char *pfx);
#define P_tmpdir
#define FOPEN_MAX
#define FILENAME_MAX
// clearerr - resets the error flags and the EOF indicator for the given file stream.
void clearerr(FILE *stream);
#define EOF (-1)
errno_t name();
// ferror - checks the given stream for errors.
int ferror(FILE *stream);
// feof - checks if the end of the given file stream has been reached.
int feof(FILE *stream);
// perror - displays a character string corresponding of the current error to stderr
void perror(const char *s);
// remove - deletes the file identified by character string pointed to by @fname.
// If the file is currently open by this or another process, the behavior of this function is implementation-defined...
// In particular, POSIX systems unlink the file name although the file system space is not reclaimed until the last running process closes the file
// NOTE: POSIX specifies many additional details for the behavior of this function.
int remove(const char *fname);
// rename - changes the filename of a file. The file is identified by character string pointed to by @old_filename.
// The new filename is identified by character string pointed to by @new_filename.
int rename(const char *old_filename, const char *new_filename);
// tmpfile - creates and opens a temporary file. The file is opened as binary file for update (as if by fopen with "wb+" mode). The filename of the file is guaranteed
// to be unique within the filesystem. At least TMP_MAX files may be opened during the lifetime of a program (this limit may be shared with tmpnam and may be further limited by FOPEN_MAX).
FILE *tmpfile(void);
errno_t tmpfile_s(FILE *restrict *restrict streamptr);
// tmpnam - creates a unique valid file name (no longer than L_tmpnam in length) and stores it in character string pointed to by @filename.
// The function is capable of generating up to TMP_MAX of unique filenames, but some or all of them may be in use in the filesystem and thus not suitable return values.
char *tmpnam(char *filename);
errno_t tmpnam_s(char *filename_s, rsize_t maxsize);
#define TMP_MAX
#define TMP_MAX_S
#define L_tmpnam
#define L_tmpnam_s
// setbuf - sets the internal buffer to use for stream operations. It should be at least BUFSIZ characters long...
void setbuf(FILE *restrict stream, char *restrict buffer);
// BUFSIZE - an integer constant expression that is good to use for the @size argument to setvbuf.
// If @buffer is not null, equivalent to  setvbuf(stream, buffer, _IOFBF, BUFSIZ)
// If @buffer is null, equivalent to  setvbuf(stream, NULL, _IONBF, 0)  which turns off buffering.
#define BUFSIZ
// setvbuf - changes the buffering mode of the given file stream @stream as indicated by the argument @mode
int setvbuf(FILE *restrict stream, char *restrict buffer, int mode, size_t size);
#define _IOFBF // full buffering
#define _IOLBF // line buffering
#define _IONBF // no buffering
// fgetc - reads the next character from the given input stream.
int fgetc(FILE *stream); 
// fgetc - also reads the next character from the given input stream, and if  getc is implemented as a macro, it may evaluate @stream more
// than once, so the corresponding argument should never be an expression with side effects.
int getc(FILE *stream);
// fgets - reads at most (@count - 1) characters from the given file stream and stores them in the
// character array pointed to by @str. Parsing stops if a newline character is found, in which case @str will contain that newline
// character, or if end-of-file occurs. If bytes are read and no errors occur, writes a null character at the position immediately
// after the last character written to @str. NOTE: POSIX additionally requires that fgets sets errno if a read error occurs.
char *fgets(char *restrict str, int count, FILE *restrict stream);
// fputc - writes a character @ch to the given output stream @stream. putc() may be implemented as a macro and evaluate @stream more
// than once, so the corresponding argument should never be an expression with side effects. Internally, the character is
// converted to unsigned char just before being written.
int fputc(int ch, FILE *stream);
int putc(int ch, FILE *stream);
// getchar - reads the next character from stdin. Equivalent to  getc(stdin)
int getchar(void);
// gets_s - reads characters from stdin until a newline is found or end-of-file occurs. Writes only at most n-1 characters into the array pointed to by str,
// and always writes the terminating null character (unless @str is a null pointer). The newline character, if found, is discarded and does not count toward the
// number of characters written to the buffer. 
char *gets_s(char *str, rsize_t n);
// ungetc - if @ch does not equal EOF, pushes the character @ch (reinterpreted as unsigned char) into the input buffer associated w/ the stream @stream in such
// a manner that subsequent read operation from @stream will retrieve that character. The external device associated with the stream is not modified.
int ungetc(int ch, FILE *stream);
// puts - writes every character from the null-terminated string @str and one additional newline character '\n' to the output stream stdout,
// as if by repeatedly executing fputc. The terminating null character from @str is not written.
int puts(const char *str);
// putchar - writes a character @ch to stdout. Internally, the character is converted to unsigned char just before being written. Equivalent to putc(ch, stdout)
int putchar(int ch);

/* @Identifier - fpos_t is a non-array complete object type, capable of uniquely specifying a position and multibyte parser state in a file
 * when you open a file for reading or writing, the initial file position is typically at the beginning of the file.
 * As you read from or write to the file, the file position advances sequentially. For example, each time you read
 * from the file, the file position moves forward by the number of bytes read.                          
 * some file access modes, such as "r+" for reading and writing, allow you to seek to specific positions within the
 * file using functions like fseek(). This enables random access to different parts of the file, allowing you to
 * read or write data at arbitrary positions.
 */ typedef fpos_t type-name;

// fopen - opens a file indicated by @filename and returns a pointer to the file stream associated with that file.
FILE *fopen(const char *restrict filename, const char *restrict mode);
/* fopen_s - is the same, except the pointer to the file stream is written to streamptr and the following
 * errors are detected at runtime and call the currently installed constraint handler function (null pointers):
 * @filename:  file name to associate the file stream to
 * @mode:      null-terminated character string determining file access mode
 * @streamptr: pointer to a pointer where the function stores the result (an out-parameter)
 *
 * File access mode strings:
 * "r" 	read 	Open a file for reading - if exists: read from start - if not: failure to open
 * "w" 	write 	Create a file for writing - if exists: destroy contents - if not: create new
 * "a" 	append 	Append to a file - if exists: write to end - if not: create new
 * "r+" read extended 	Open a file for read/write - if exists: read from start - if not: error
 * "w+" write extended 	 Create a file for read/write - if exists: destroy contents - if not: create new
 * "a+" append extended 	Open a file for read/write - if exists: write to end - if not: create new 
 * '+' is update mode, which allows both input and output operations to be performed on the file.
 */
errno_t fopen_s(FILE *restrict *restrict streamptr, const char *restrict filename, const char *restrict mode);
// freopen - first, attempts to close the file associated with stream, ignoring any errors...
// Then, if @filename is not null, attempts to open the file specified by @filename using @mode as if by fopen, and associates that file with the file stream pointed to by @stream.
// If @filename is a null pointer, then the function attempts to reopen the file that is already associated with @stream.
// It is implementation defined which mode changes are allowed in this case.
FILE *freopen(const char *restrict filename, const char *restrict mode, FILE *restrict stream);
// freopen_s - mode is treated as in  fopen_s and that the pointer to the file stream is written to @newstreamptr
// and the following errors are detected at runtime and call the currently installed constraint handler function (null pointers):
// @newstreamptr, @stream and @mode (as we have already explained)
errno_t freopen_s(FILE *restrict *restrict newstreamptr, const char *restrict filename, const char *restrict mode, FILE *restrict stream);
// fclose - closes the given file stream. Any unwritten buffered data are flushed to the OS. Any unread buffered data are discarded.
// Whether or not the operation succeeds, the stream is no longer associated with a file, and the buffer allocated by setbuf
// or setvbuf, if any, is also disassociated and deallocated if automatic allocation was used.
int fclose(FILE *stream);
// fflush - for output streams (and for update streams on which the last operation was output), writes any unwritten data from the stream's buffer to the associated output device.
// For input streams (and for update streams on which the last operation was input), the behavior is undefined.
// If @stream is a null pointer, all open output streams are flushed, including the ones manipulated within library
// packages or otherwise not directly accessible to the program. NOTE: POSIX extends the specification of fflush by defining its effects on an input stream
int fflush(FILE *stream);
// fread - reads up to @count objects into the array buffer from the given input stream @stream as if by
// calling fgetc @size times for each object, and storing the results, in the order obtained, into the successive positions of @buffer, which is reinterpreted
// as an array of unsigned char. The file position indicator for the stream is advanced by the number of characters read.
size_t fread(void *restrict buffer, size_t size, size_t count, FILE *restrict stream);
// fwrite - writes @count of objects from the given array buffer to the output stream @stream.
// The objects are written as if by reinterpreting each object as an array of unsigned char and calling fputc size times for each object to write those
// unsigned chars into @stream, in order. The file position indicator for the stream is advanced by the number of characters written.
size_t fwrite(const void *restrict buffer, size_t size, size_t count, FILE *restrict stream);
// scanf_s - reads data from a variety of sources, interprets it according to @format and stores and the results into given locations: from stdin...
int scanf_s(const char *restrict format, ...);
int fscanf_s(FILE *restrict stream, const char *restrict format, ...); // from file stream @stream
// sscanf_s - reaching the end of the string is equivalent to reaching the end-of-file condition for fscanf 
int sscanf_s(const char *restrict buffer, const char *restrict format, ...); // from a null-terminated character string buffer.
// vcsanf - reads data from the a variety of sources, interprets it according to @format and stores the results into locations defined by @vlist.
int vscanf(const char *restrict format, va_list vlist);
int vfscanf(FILE *restrict stream, const char *restrict format, va_list vlist);
int vsscanf(const char *restrict buffer, const char *restrict format, va_list vlist);
int vscanf_s(const char *restrict format, va_list vlist);
int vfscanf_s(FILE *restrict stream, const char *restrict format, va_list vlist);
int vsscanf_s(const char *restrict buffer, const char *restrict format, va_list vlist);
int snprintf(char *restrict buffer, size_t bufsz, const char *restrict format, ...);
// printf_s - writes the results to a character string @buffer. At most @bufsz - 1 characters are written. The resulting character string will be terminated
// with a null character, unless @bufsz is zero. If @bufsz is zero, nothing is written and @buffer may be a null pointer, however the return value (number of bytes that
// would be written not including the null terminator) is still calculated and returned.
int printf_s(const char *restrict format, ...);
int fprintf_s(FILE *restrict stream, const char *restrict format, ...);
int sprintf_s(char *restrict buffer, rsize_t bufsz, const char *restrict format, ...);
int snprintf_s(char *restrict buffer, rsize_t bufsz, const char *restrict format, ...);
int vsnprintf(char *restrict buffer, size_t bufsz, const char *restrict format, va_list vlist);
// vprintf_s - loads the data from the locations, defined by @vlist, converts them to character string equivalents and writes the results to a variety of sinks.
int vprintf_s(const char *restrict format, va_list vlist);
int vfprintf_s(FILE *restrict stream, const char *restrict format, va_list vlist);
int vsprintf_s(char *restrict buffer, rsize_t bufsz, const char *restrict format, va_list vlist);
int vsnprintf_s(char *restrict buffer, rsize_t bufsz, const char *restrict format, va_list vlist);
// ftell - returns the file position indicator for the file stream @stream.
// If the stream is open in binary mode, the value obtained by this function is the number of bytes from the beginning of the file.
// If the stream is open in text mode, the value returned by this function is unspecified and is only meaningful as the input to fseek().
long ftell(FILE *stream);
// fgetpos - obtains the file position indicator and the current parse state (if any) for the file stream @stream
// and stores them in the object pointed to by pos. The value stored is only meaningful as the input to fsetpos.
int fgetpos(FILE *restrict stream, fpos_t *restrict pos);
// fsetpos - sets the file position indicator and the multibyte parsing state (if any) for the file stream @stream according to the value pointed to by @pos.
// Besides establishing new parse state and position, a call to this function undoes the effects of ungetc and clears the end-of-file state, if it is set.
int fsetpos( FILE *stream, const fpos_t *pos );
// fseek - sets the file position indicator for the file stream @stream to the value pointed to by @offset.
// If the stream is open in binary mode, the new position is exactly @offset bytes measured from the beginning of the file
// if @origin is SEEK_SET, from the current file position if @origin is SEEK_CUR, and from the end of the file if @origin is
// SEEK_END. Binary streams are not required to support SEEK_END, in particular if additional null bytes are output.
int fseek(FILE *stream, long offset, int origin);
// SEEK_SET - if the stream is open in text mode, the only supported values for @offset are zero (which works with any origin)
// and a value returned by an earlier call to ftell on a stream associated with the same file (which only works with @origin of SEEK_SET).
#define SEEK_SET
// SEEK_CUR - if the stream is wide-oriented, the restrictions of both text and binary streams apply (result of ftell is allowed with SEEK_SET
// and zero offset is allowed from SEEK_SET and SEEK_CUR, but not SEEK_END).
#define SEEK_CUR
// SEEK_END - in addition to changing the file position indicator, fseek undoes the effects of ungetc and clears the end-of-file status, if applicable.
#define SEEK_END
// rewind - moves the file position indicator to the beginning of the given file stream.
// The function is equivalent to fseek(stream, 0, SEEK_SET);, except that end-of-file and error indicators are cleared.
void rewind(FILE *stream)

#include <stdint.h>
#define UINT64_C // Example macro is a portable, specific macro allowing you to create integer constants of specific types 
typedef int8_t  type-name // And those types from  stdint.h
typedef int16_t type-name
typedef int32_t type-name
typedef int64_t type-name
typedef int_fast8_t type-name
typedef int_fast16_t type-name
typedef int_fast32_t type-name
typedef int_fast64_t type-name
typedef int_least8_t type-name
typedef int_least16_t type-name
typedef int_least32_t type-name
typedef int_least64_t type-name
typedef intmax_t type-name
typedef intptr_t type-name
typedef uint8_t type-name
typedef uint16_t type-name
typedef uint32_t type-name
typedef uint64_t type-name
typedef uint_fast8_t type-name
typedef uint_fast16_t type-name
typedef uint_fast32_t type-name
typedef uint_fast64_t type-name
typedef uint_least8_t type-name
typedef uint_least16_t type-name
typedef uint_least32_t type-name
typedef uint_least64_t type-name
typedef uintmax_t type-name
typedef uintptr_t type-name
#define INT8_WIDTH
#define INT16_WIDTH
#define INT32_WIDTH
#define INT64_WIDTH
#define INT_FAST8_WIDTH
#define INT_FAST16_WIDTH
#define INT_FAST32_WIDTH
#define INT_FAST64_WIDTH
#define INT_LEAST8_WIDTH
#define INT_LEAST16_WIDTH
#define INT_LEAST32_WIDTH
#define INT_LEAST64_WIDTH
#define INTPTR_WIDTH
#define INTMAX_WIDTH
#define INT8_MIN
#define INT16_MIN
#define INT32_MIN
#define INT64_MIN
#define INT_FAST8_MIN
#define INT_FAST16_MIN
#define INT_FAST32_MIN
#define INT_FAST64_MIN
#define INT_LEAST8_MIN
#define INT_LEAST16_MIN
#define INT_LEAST32_MIN
#define INT_LEAST64_MIN
#define INTPTR_MIN
#define INTMAX_MIN
#define INT8_MAX
#define INT16_MAX
#define INT32_MAX
#define INT64_MAX
#define INT_FAST8_MAX
#define INT_FAST16_MAX
#define INT_FAST32_MAX
#define INT_FAST64_MAX
#define INT_LEAST8_MAX
#define INT_LEAST16_MAX
#define INT_LEAST32_MAX
#define INT_LEAST64_MAX
#define INTPTR_MAX
#define INTMAX_MAX
#define UINT8_WIDTH
#define UINT16_WIDTH
#define UINT32_WIDTH
#define UINT64_WIDTH
#define UINT_FAST8_WIDTH
#define UINT_FAST16_WIDTH
#define UINT_FAST32_WIDTH
#define UINT_FAST64_WIDTH
#define UINT_LEAST8_WIDTH
#define UINT_LEAST16_WIDTH
#define UINT_LEAST32_WIDTH
#define UINT_LEAST64_WIDTH
#define UINTPTR_WIDTH
#define UINTMAX_WIDTH
#define UINT8_MAX
#define UINT16_MAX
#define UINT32_MAX
#define UINT64_MAX
#define UINT_FAST8_MAX
#define UINT_FAST16_MAX
#define UINT_FAST32_MAX
#define UINT_FAST64_MAX
#define UINT_LEAST8_MAX
#define UINT_LEAST16_MAX
#define UINT_LEAST32_MAX
#define UINT_LEAST64_MAX
#define UINTPTR_MAX
#define UINTMAX_MAX
// RSIZE_MAX - largest acceptable size for bounds-checked functions, expands to either constant or variable which may change at runtime
// (e.g. as the currently allocated memory size changes)
#define RSIZE_MAX
#define PTRDIFF_WIDTH // bit width of object of ptrdiff_t type
#define PTRDIFF_MIN // minimum value of ptrdiff_t
#define PTRDIFF_MAX // maximum value of ptrdiff_t
#define SIZE_WIDTH // bit width of object of size_t type
#define SIZE_MAX // maximum value of size_t
#define SIG_ATOMIC_WIDTH // bit width of object of sig_atomic_t type
#define SIG_ATOMIC_MIN // minimum value of sig_atomic_t
#define SIG_ATOMIC_MAX // maximum value of sig_atomic_t
#define WINT_WIDTH // bit width of object of wint_t type
#define WINT_MIN // minimum value of wint_t
#define WINT_MAX // maximum value of wint_t
#define WCHAR_WIDTH
#define WCHAR_MIN
#define WCHAR_MAX
#define stdin  /* implementation-defined */
#define stdout /* implementation-defined */
#define stderr /* implementation-defined */

#include <stddef.h>
// size_t - unsigned integer type returned by the sizeof operator.
typedef ptrdiff_t type-name;
void safe_copy(void *restrict dest, const void *restrict src, rsize_t n); // note: non-standard function
typedef size_t rsize_t; // associated with safe_copy and other secure functions
#define offsetof(type, member)
// ptrdiff_t - signed integer type returned when subtracting two pointers.
typedef ptrdiff_t;
// nullptr_t - the type of the predefined null pointer constant nullptr.
typedef typeof(nullptr) nullptr_t;
typedef max_align_t;
#define NULL
// The function-like macro unreachable expands to a void expression. Executing unreachable() results in undefined behavior.
// An implementation may use this to optimize impossible code branches away (typically, in optimized builds) or to trap them to prevent further execution (typically, in debug builds).
#define unreachable()

#include <stdarg.h>
void va_start(va_list ap, ...);
/* va_arg - macro expands to an expression of type T that corresponds to the next parameter from the va_list @ap
 * Prior to calling va_arg, @ap  must be initialized by a call to either va_start or va_copy, with no intervening
 * call to va_end. Each invocation of the va_arg macro modifies ap to point to the next variable argument.
 * If the type of the next argument in ap (after promotions) is not compatible with T, the behavior is undefined, unless:
 * one type is a signed integer type, the other type is the corresponding unsigned integer type, and the value is represent
 * -able in both types; or one type is pointer to void and the other is a pointer to a character type */
T va_arg(va_list ap, T);
void va_copy(va_list dest, va_list src); // copies @src to @dest
void va_end(va_list ap); // performs cleanup for an @ap object and may render it unusable

#include <uchar.h> 
typedef unsigned char char8_t;
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
// mbrtoc8 - converts a narrow multibyte character to UTF-8 encoding.
size_t mbrtoc8(char8_t *restrict pc8, const char *restrict s, size_t n, mbstate_t *restrict ps);
// c8rtomb - converts a single code point from UTF-8 to a narrow multibyte character representation.
size_t c8rtomb(char *restrict s, char8_t c8, mbstate_t *restrict ps);
// mbrtoc16 - converts a single code point from its narrow multibyte character
// representation to its variable-length 16-bit wide character representation (typically, UTF-16).
size_t mbrtoc16(char16_t *restrict pc16, const char *restrict s, size_t n, mbstate_t *restrict ps);
// c16rtomb - converts a single code point from its variable-length 16-bit wide character representation (typically, UTF-16)
// to its narrow multibyte character representation.
size_t c16rtomb(char *restrict s, char16_t c16, mbstate_t *restrict ps);
// mbrtoc32 - converts a single code point from its narrow multibyte character representation to its
// variable-length 32-bit wide character representation (but typically, UTF-32).
size_t mbrtoc32(char32_t restrict *pc32, const char *restrict s, size_t n, mbstate_t *restrict ps);
// c32rtomb - converts a single code point from its variable-length 32-bit wide character representation (but typically, UTF-32)
// to its narrow multibyte character representation.
size_t c32rtomb(char *restrict s, char32_t c32, mbstate_t *restrict ps);

#include <wchar.h> 
// wchar_t - integer type that can hold any valid wide character.
// mbstate_t - is a trivial non-array type that can represent any of the conversion states that can occur in an implementation-defined set of
// supported multibyte character encoding rules.
struct mbstate_t;
// mbsinit - if @ps is not a null pointer, the mbsinit function determines whether the pointed-to mbstate_t object describes the initial conversion state.
int mbsinit(const mbstate_t *ps);
// fwide - if mode > 0, attempts to make @stream wide-oriented. If mode < 0, attempts to make @stream byte-oriented.
// If mode==0, only queries the current orientation of the stream.
int fwide(FILE *stream, int mode);
// btowc - widens a single-byte character @c (reinterpreted as unsigned char) to its wide character equivalent.
// Most multibyte character encodings use single-byte codes to represent the characters from the ASCII character set. This function may be used to convert such characters to wchar_t.
wint_t btowc(int c);
// wctob - narrows a wide character @c if its multibyte character equivalent in the initial shift state is a single byte.
// This is typically possible for the characters from the ASCII character set, since most multibyte encodings (such as UTF-8) use single bytes to encode those characters.
int wctob(wint_t c);
// mbrlen - determines the size, in bytes, of the representation of a multibyte character.
size_t mbrlen(const char *restrict s, size_t n, mbstate_t *restrict ps);
// mbrtowc - converts a narrow multibyte character to its wide character representation.
size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
size_t mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n, mbstate_t *restrict ps);
// wcrtomb - converts a wide character to its narrow multibyte representation.
size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps);
size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps);
errno_t wcrtomb_s(size_t *restrict retval, char *restrict s, rsize_t ssz, wchar_t wc, mbstate_t *restrict ps);
// mbsrtowcs - converts a null-terminated multibyte character sequence, which begins in the conversion state
// described by @ps, from the array whose first element is pointed to by @src to its wide character representation. If @dst is not null, converted characters are stored in the
// successive elements of the wchar_t array pointed to by @dst. No more than @len wide characters are written to the destination array.
// Each multibyte character is converted as if by a call to mbrtowc.
size_t mbsrtowcs(wchar_t *restrict dst, const char **restrict src, size_t len, mbstate_t *restrict ps);
errno_t mbsrtowcs_s(size_t *restrict retval, wchar_t *restrict dst, rsize_t dstsz, const char **restrict src, rsize_t len, mbstate_t *restrict ps);
// wcsrtombs - converts a sequence of wide characters from the array whose first element is pointed to by @src
// to its narrow multibyte representation that begins in the conversion state described by @ps. If @dst is not null, converted characters are stored in the successive elements of the
// char array pointed to by @dst. No more than @len bytes are written to the destination array. Each character is converted as if by a call to wcrtomb.
size_t wcsrtombs(char *restrict dst, const wchar_t **restrict src, size_t len, mbstate_t *restrict ps);
errno_t wcsrtombs_s(size_t *restrict retval, char *restrict dst, rsize_t dstsz, const wchar_t **restrict src, rsize_t len, mbstate_t *restrict ps);
wint_t getwchar(void); // Reads the next wide character from stdin.
// fgetwc - reads the next wide character from the given input stream. getwc() may be implemented as a macro and may evaluate @stream more than once.
wint_t fgetwc(FILE *stream);
wint_t getwc(FILE *stream);
// fgetws - reads at most (count - 1) wide characters from the given file stream and stores them in @str.
// The produced wide string is always null-terminated. Parsing stops if end-of-file occurs or a newline wide character is found, in which case
// @str will contain that wide newline character.
wchar_t *fgetws(wchar_t *restrict str, int count, FILE *restrict stream);
// ungetwc - if @ch does not equal WEOF, pushes the wide character @ch into the input buffer associated with the stream @stream in such a manner
// that subsequent read operation from @stream will retrieve that wide character. The external device associated with the stream is not modified.
wint_t ungetwc(wint_t ch, FILE *stream);
wint_t putwchar(wchar_t ch); // Writes a wide character @ch to stdout.
// fputwc - writes a wide character @ch to the given output stream @stream.
wint_t fputwc(wchar_t ch, FILE *stream);
wint_t putwc(wchar_t ch, FILE *stream);
// putwc - writes every character from the null-terminated wide string @str to the output stream @stream, as if by repeatedly executing fputwc.
// The terminating null wide character from @str is not written.
int fputws(const wchar_t *restrict str, FILE *restrict stream);
// wscanf - reads data from the a variety of sources, interprets it according to format and stores the results into given locations.
int wscanf(const wchar_t *restrict format, ...);
int fwscanf(FILE *restrict stream, const wchar_t *restrict format, ...);
int swscanf(const wchar_t *restrict buffer, const wchar_t *restrict format, ...);
int wscanf_s(const wchar_t *restrict format, ...);
int fwscanf_s(FILE *restrict stream, const wchar_t *restrict format, ...);
int swscanf_s(const wchar_t *restrict s, const wchar_t *restrict format, ...);
int vwscanf(const wchar_t *restrict format, va_list vlist);
int vfwscanf(FILE *restrict stream, const wchar_t *restrict format, va_list vlist);
int vswscanf(const wchar_t *restrict buffer, const wchar_t *restrict format, va_list vlist);
int vwscanf_s(const wchar_t *restrict format, va_list vlist);
int vfwscanf_s(FILE *restrict stream, const wchar_t *restrict format, va_list vlist);
int vswscanf_s(const wchar_t *restrict buffer, const wchar_t *restrict format, va_list vlist);
// wprintf - loads the data from the given locations, converts them to wide string equivalents and writes the results to a variety of sinks.
int wprintf(const wchar_t *restrict format, ...);
int fwprintf(FILE *restrict stream, const wchar_t *restrict format, ...);
int swprintf(wchar_t *restrict buffer, size_t bufsz, const wchar_t *restrict format, ...);
int wprintf_s(const wchar_t *restrict format, ...);
int fwprintf_s(FILE *restrict stream, const wchar_t *restrict format, ...);
int swprintf_s(wchar_t *restrict buffer, rsize_t bufsz, const wchar_t *restrict format, ...);
int snwprintf_s(wchar_t *restrict s, rsize_t n, const wchar_t *restrict format, ...);
// vwprintf - loads the data from locations, defined by vlist, converts them to wide string equivalents and writes the results to a variety of sinks.
int vwprintf(const wchar_t *restrict format, va_list vlist);
int vfwprintf(FILE *restrict stream, const wchar_t *restrict format, va_list vlist);
int vswprintf(wchar_t *restrict buffer, size_t bufsz, const wchar_t *restrict format, va_list vlist);
int vwprintf_s(const wchar_t *restrict format, va_list vlist);
int vfwprintf_s(FILE *restrict stream, const wchar_t *restrict format, va_list vlist);
int vswprintf_s(wchar_t *restrict buffer, rsize_t bufsz, const wchar_t *restrict format, va_list vlist);
int vsnwprintf_s(wchar_t *restrict buffer, rsize_t bufsz, const wchar_t *restrict format, va_list vlist);
// wcscpy - copies one wide string to another.
wchar_t *wcscpy(wchar_t *restrict dest, const wchar_t *restrict src);
errno_t wcscpy_s(wchar_t *restrict dest, rsize_t destsz, const wchar_t *restrict src);
// wcsncpy - copies a certain amount of wide characters from one string to another.
wchar_t *wcsncpy(wchar_t *restrict dest, const wchar_t *restrict src, size_t count);
errno_t wcsncpy_s(wchar_t *restrict dest, rsize_t destsz, const wchar_t *restrict src, rsize_t count);
// wcscat - appends a copy of one wide string to another.
wchar_t *wcscat(wchar_t *restrict dest, const wchar_t *restrict src);
errno_t wcscat_s(wchar_t *restrict dest, rsize_t destsz, const wchar_t *restrict src);
// wcsncat - appends a certain amount of wide characters from one wide string to another.
wchar_t *wcsncat(wchar_t *restrict dest, const wchar_t *restrict src, size_t count);
errno_t wcsncat_s(wchar_t *restrict dest, rsize_t destsz, const wchar_t *restrict src, rsize_t count);
// transform a wide string so that wcscmp would produce the same result as wcscoll.
size_t wcsxfrm(wchar_t *restrict dest, const wchar_t *restrict src, size_t count);
// wcslen - returns the length of a wide string.
size_t wcslen(const wchar_t *str);
size_t wcsnlen_s(const wchar_t *str, size_t strsz);
// wcscmp - compares two wide strings.
int wcscmp(const wchar_t *lhs, const wchar_t *rhs);
// wcsncmp - compares a certain amount of characters from two wide strings.
int wcsncmp(const wchar_t *lhs, const wchar_t *rhs, size_t count);
// wcscoll - compares two wide strings in accordance to the current locale.
int wcscoll(const wchar_t *lhs, const wchar_t *rhs);
// wcschr - finds the first occurrence of a wide character in a wide string.
wchar_t *wcschr(const wchar_t *str, wchar_t ch);
/*QWchar_t*/ *wcschr( /*QWchar_t*/ *str, wchar_t ch); // type-generic function equivalent to the original wcschr.
// wcsrchr - finds the last occurrence of a wide character in a wide string.
wchar_t *wcsrchr(const wchar_t *str, wchar_t ch);
/*QWchar_t*/ *wcsrchr( /*QWchar_t*/ *str, wchar_t ch);
// wcsspn - returns the length of the maximum initial segment that consists of only the wide characters found in another wide string.
size_t wcsspn(const wchar_t *dest, const wchar_t *src);
// wcscspn - returns the length of the maximum initial segment that consists of only the wide chars not found in another wide string.
size_t wcscspn(const wchar_t *dest, const wchar_t *src);
// wcspbrk - finds the first location of any wide character in one wide string, in another wide string.
wchar_t *wcspbrk(const wchar_t *dest, const wchar_t *str);
/*QWchar_t*/ *wcspbrk( /*QWchar_t*/ *dest, const wchar_t *str);
// wcsstr - finds the first occurrence of a wide string within another wide string.
wchar_t *wcsstr(const wchar_t *dest, const wchar_t *src);
/*QWchar_t*/ *wcsstr( /*QWchar_t*/ *dest, const wchar_t *src);
// wcstok - finds the next token in a wide string.
wchar_t *wcstok(wchar_t *restrict str, const wchar_t *restrict delim, wchar_t **restrict ptr);
wchar_t *wcstok_s(wchar_t *restrict str, rsize_t *restrict strmax, const wchar_t *restrict delim, wchar_t **restrict ptr);
// wcstol - converts a wide string to an integer value.
long wcstol(const wchar_t *restrict str, wchar_t **restrict str_end, int base);
long long wcstoll(const wchar_t *restrict str, wchar_t **restrict str_end, int base);
// wcstoul - converts a wide string to an unsigned integer value.
unsigned long wcstoul(const wchar_t *restrict str, wchar_t **restrict str_end, int base);
unsigned long long wcstoull(const wchar_t *restrict str, wchar_t **restrict str_end, int base);
// wcstof - converts a wide string to a floating-point value.
float wcstof(const wchar_t *restrict str, wchar_t **restrict str_end);
double wcstod(const wchar_t *restrict str, wchar_t **restrict str_end);
long double wcstold(const wchar_t *restrict str, wchar_t **restrict str_end);
// wmemcpy - copies a certain amount of wide characters between two non-overlapping arrays.
wchar_t *wmemcpy(wchar_t *restrict dest, const wchar_t *restrict src, size_t count);
errno_t wmemcpy_s(wchar_t *restrict dest, rsize_t destsz, const wchar_t *restrict src, rsize_t count);
// wmemmove - copies a certain amount of wide characters between two, possibly overlapping, arrays
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t count);
errno_t wmemmove_s(wchar_t *dest, rsize_t destsz, const wchar_t *src, rsize_t count);
// wmemcmp - compares a certain amount of wide characters from two arrays.
int wmemcmp(const wchar_t *lhs, const wchar_t *rhs, size_t count);
// wmemchr - finds the first occurrence of a wide character in a wide character array.
wchar_t *wmemchr(const wchar_t *ptr, wchar_t ch, size_t count);
/*QWchar_t*/ *wmemchr( /*Qwchar_t*/ *ptr, wchar_t ch, size_t count);
// wmemset - copies the given wide character to every position in a wide character array.
wchar_t *wmemset(wchar_t *dest, wchar_t ch, size_t count);

#include <wctype.h>
// WEOF - a non-character value of type wint_t used to indicate errors.
// wint_t - integer type that can hold any valid wide character and at least one more value.
// wctrans_t - scalar type that holds locale-specific character mapping.
// wctype - constructs a value of type wctype_t that describes a LC_CTYPE category of wide character classification.
wctype_t wctype(const char* str);
// iswalnum - checks if the given wide character is an alphanumeric character.
int iswalnum(wint_t ch);
// iswalpha - checks if the given wide character is an alphabetic character.
int iswalpha(wint_t ch);
// iswlower - checks if a wide character is an lowercase character.
int iswlower(wint_t ch);
// iswupper - checks if a wide character is an uppercase character.
int iswupper(wint_t ch);
// iswdigit - checks if a wide character is a digit.
int iswdigit(wint_t ch);
// iswxdigit - checks if a wide character is a hexadecimal character.
int iswxdigit(wint_t ch);
// iswcntrl - checks if a wide character is a control character. 
int iswcntrl(wint_t ch);
// iswgraph - checks if a wide character is a graphical character.
int iswgraph(wint_t ch);
// iswspace - checks if a wide character is a space character.
int iswspace(wint_t ch);
// iswblank - checks if a wide character is a blank character.
int iswblank(wint_t ch);
// iswprint - checks if a wide character is a printing character.
int iswprint(wint_t ch);
// iswpunct - checks if a wide character is a punctuation character 
int iswpunct(wint_t ch);
// iswctype - classifies a wide character according to the specified LC_CTYPE category.
int iswctype(wint_t wc, wctype_t desc);
// LC_CTYPE - selects the character classification category of the C locale 
#define LC_CTYPE    /*implementation defined*/
// towlower - converts a wide character to lowercase.
wint_t towlower(wint_t wc);
// towupper - converts a wide character to uppercase.
wint_t towupper(wint_t wc);
// towctrans - performs character mapping according to the specified LC_CTYPE mapping category.
wint_t towctrans(wint_t wc, wctrans_t desc);
// wctrans - looks up a character mapping category in the current C locale.
wctrans_t wctrans(const char* str);

#include <string.h>
double atof(const char *str);
int atoi(const char *str);
long long atoll(const char *str);
long strtol(const char *restrict str, char **restrict str_end, int base);
long long strtoll(const char *restrict str, char **restrict str_end, int base);
unsigned long strtoul(const char *restrict str, char **restrict str_end, int base);
unsigned long long strtoull(const char *restrict str, char **restrict str_end, int base);
float strtof(const char *restrict str, char **restrict str_end);
double strtod(const char *restrict str, char **restrict str_end);
long double strtold(const char *restrict str, char **restrict str_end);
int strfromf(char *restrict s, size_t n, const char *restrict format, float fp);
int strfromd(char *restrict s, size_t n, const char *restrict format, double fp);
int strfroml(char *restrict s, size_t n, const char *restrict format, long double fp);
char *strcpy(char *restrict dest, const char *restrict src);
errno_t strcpy_s(char *restrict dest, rsize_t destsz, const char *restrict src);
char *strncpy(char *restrict dest, const char *restrict src, size_t count);
errno_t strncpy_s(char *restrict dest, rsize_t destsz, const char *restrict src, rsize_t count);
char *strcat(char *restrict dest, const char *restrict src );
errno_t strcat_s(char *restrict dest, rsize_t destsz, const char *restrict src);
char *strncat(char *restrict dest, const char *restrict src, size_t count);
errno_t strncat_s(char *restrict dest, rsize_t destsz, const char *restrict src, rsize_t count);
size_t strxfrm(char *restrict dest, const char *restrict src, size_t count);
char *strdup(const char *src);
char *strndup(const char *src, size_t size);
intmax_t strtoimax(const char *restrict nptr, char **restrict endptr, int base);
uintmax_t strtoumax(const char *restrict nptr, char **restrict endptr, int base);
size_t strlen(const char *str);
size_t strnlen_s(const char *str, size_t strsz);
int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, size_t count);
int strcoll(const char* lhs, const char *rhs);
char *strchr(const char *str, int ch);
char *strrchr(const char *str, int ch);
size_t strspn(const char *dest, const char *src);
size_t strcspn(const char *dest, const char *src);
char *strpbrk(const char *dest, const char *breakset);
char *strstr(const char *str, const char *substr);
char *strtok(char *restrict str, const char *restrict delim);
char *strtok_s(char *restrict str, rsize_t *restrict strmax, const char *restrict delim, char **restrict ptr);
void *memchr(const void *ptr, int ch, size_t count);
int memcmp(const void *lhs, const void *rhs, size_t count);
void *memset(void *dest, int ch, size_t count);
void *memset_explicit(void *dest, int ch, size_t count);
errno_t memset_s(void *dest, rsize_t destsz, int ch, rsize_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memcpy(void *restrict dest, const void *restrict src, size_t count);
errno_t memcpy_s(void *restrict dest, rsize_t destsz, const void *restrict src, rsize_t count);
void *memccpy(void *restrict dest, const void *restrict src, int c, size_t count);
void *memmove(void *dest, const void *src, size_t count);
errno_t memmove_s(void *dest, rsize_t destsz, const void *src, rsize_t count);
char *strerror(int errnum);
errno_t strerror_s(char *buf, rsize_t bufsz, errno_t errnum);
size_t strerrorlen_s(errno_t errnum);

#include <limits.h>
// MB_LEN_MAX - maximum number of bytes in a multibyte character, for any supported locale
#define MB_LEN_MAX
#define BOOL_WIDTH
#define CHAR_BIT   // number of bits in a byte 
#define MB_LEN_MAX // maximum number of bytes in a multibyte character
#define CHAR_WIDTH // bit width of char, same as CHAR_BIT
#define CHAR_MIN
#define CHAR_MAX
#define SCHAR_WIDTH
#define SHRT_WIDTH
#define INT_WIDTH
#define LONG_WIDTH
#define LLONG_WIDTH
#define SCHAR_MIN
#define SHRT_MIN
#define INT_MIN
#define LONG_MIN
#define LLONG_MIN
#define SCHAR_MAX
#define SHRT_MAX
#define INT_MAX
#define LONG_MAX
#define LLONG_MAX
#define UCHAR_WIDTH
#define USHRT_WIDTH
#define UINT_WIDTH
#define ULONG_WIDTH
#define ULLONG_WIDTH
#define UCHAR_MAX
#define USHRT_MAX
#define UINT_MAX
#define ULONG_MAX
#define ULLONG_MAX
// BITINT_MAXWIDTH - maximum width N supported by the declaration of a bit-precise integer in the type specifier _BitInt(N), greater than or equal to ULLONG_WIDTH
#define BITINT_MAXWIDTH

#include <inttypes.h>
intmax_t imaxabs(intmax_t n);
imaxdiv_t imaxdiv(intmax_t x, intmax_t y);

#include <math.h>
#define INFINITY i /*implementation defined*/
// MATH_ERRNO - indicates that floating-point operations use the variable errno to report errors. 
#define MATH_ERRNO        1
// MATH_ERREXCEPT - indicates that floating-point exceptions are used: at least FE_DIVBYZERO, FE_INVALID, and FE_OVERFLOW are defined in <fenv.h>
#define MATH_ERREXCEPT    2
// math_errhandling - the macro constant math_errhandling expands to an expression of type int that is either equal to MATH_ERRNO, or equal to MATH_ERREXCEPT,
// or equal to their bitwise OR (MATH_ERRNO | MATH_ERREXCEPT).
#define math_errhandling
// FP_NORMAL - indicates that the value is normal, i.e. not an infinity, subnormal, not-a-number or zero
#define FP_NORMAL
// FP_NORMAL - indicates that the value is subnormal (the subset of denormalized numbers that fill the underflow gap around zero in floating-point)
#define FP_SUBNORMAL
// FP_ZERO - indicates that the value is positive or negative zero
#define FP_ZERO
// FP_INFINITE - indicates that the value is not representable by the underlying type (positive or negative infinity)
#define FP_INFINITE
// FP_NAN - indicates that the value is not-a-number (NaN) 
#define FP_NAN
// ilogbf - extracts the value of the unbiased exponent from the floating-point argument @arg, and returns it as a signed integer value.
int ilogbf(float arg);
int ilogb(double arg);
int ilogbl(long double arg);
// FP_ILOGB0 - expands to integer constant expression whose value is either INT_MIN or -INT_MAX.
#define FP_ILOGB0
// FP_ILPGBNAN - expands to integer constant expression whose value is either INT_MIN or +INT_MAX.
#define FP_ILOGBNAN
// nanf - converts the implementation-defined character string @arg into the corresponding quiet NaN value
float       nanf(const char *arg);
double      nan(const char *arg);
long double nanl(const char *arg);
_Decimal32  nand32(const char *arg);
_Decimal64  nand64(const char *arg);
_Decimal128 nand128(const char *arg);
// fmaf - computes (x*y) + z   as if to infinite precision and rounded only once to fit the result type.
float       fmaf(float x, float y, float z);
double      fma(double x, double y, double z);
long double fmal(long double x, long double y, long double z);
// FP_FAST_FMA - if the macro constants FP_FAST_FMA, FP_FAST_FMAF, or FP_FAST_FMAL are defined, the corresponding function fmaf, fma, or fmal evaluates faster..
// (in addition to being more precise) than the expression (x*y)+z for float, double, and long double arguments, respectively. If defined, these macros evaluate to integer 1.
#define FP_FAST_FMA
#define FP_FAST_FMAF
#define FP_FAST_FMAL
// HUGE_VALF - expand to positive floating point constant expressions which compare equal to the values returned by floating-point functions and operators in case of overflow
#define HUGE_VALF
#define HUGE_VAL
#define HUGE_VALL
typedef float_t
typedef double_t
float fabsf(float arg);
double fabs(double arg);
long double fabsl(long double arg);
_Decimal32  fabsd32(_Decimal32 arg);
_Decimal64  fabsd64( _Decimal64 arg );
_Decimal128 fabsd128( _Decimal128 arg );
float       fmodf(float x, float y);
double      fmod(double x, double y);
long double fmodl(long double x, long double y);
float       remainderf(float x, float y);
double      remainder(double x, double y);
long double remainderl(long double x, long double y);
float       remquof(float x, float y, int *quo);
double      remquo(double x, double y, int *quo);
long double remquol(long double x, long double y, int *quo);
float       fmaf(float x, float y, float z);  
double      fma(double x, double y, double z);
long double fmal(long double x, long double y, long double z);
float       fmaxf(float x, float y);
double      fmax(double x, double y);
long double fmaxl(long double x, long double y);
float       fminf(float x, float y);
double      fmin(double x, double y);
long double fminl(long double x, long double y);
float       fdimf(float x, float y);
double      fdim(double x, double y);
long double fdiml(long double x, long double y);
float       nanf(const char *arg);
double      nan(const char *arg);
long double nanl(const char *arg);
_Decimal32  nand32(const char *arg);
_Decimal64  nand64(const char *arg);
_Decimal128 nand128(const char *arg );
float       expf(float arg);
double      exp(double arg);
long double expl(long double arg);
float       exp2f(float n);
double      exp2(double n);
long double exp2l(long double n);
float       expm1f(float arg);
double      expm1(double arg);
long double expm1l(long double arg);
float       logf(float arg);
double      log(double arg);
long double logl(long double arg);
float       log10f(float arg);
double      log10(double arg);
long double log10l(long double arg);
float       log2f(float arg);
double      log2(double arg);
long double log2l(long double arg);
float       log1pf(float arg);
double      log1p(double arg);
long double log1pl(long double arg);
float       logbf(float arg);
double      logb(double arg);
long double logbl(long double arg);
float powf(float base, float exponent);
double pow(double base, double exponent);
long double powl(long double base, long double exponent);
float       sqrtf(float arg);
double      sqrt(double arg);
long double sqrtl(long double arg);
float       cbrtf(float arg);
double      cbrt(double arg);
long double cbrtl(long double arg);
// hypotf - computes the square root of the sum of the squares of @x and @y, without undue overflow or underflow at intermediate stages of the computation.
float       hypotf(float x, float y);
double      hypot(double x, double y);
long double hypotl(long double x, long double y);
// erff - a mathematical function that describes the probability of a normally distributed random variable falling within a certain range of values.
float       erff(float arg);
double      erf(double arg);
long double erfl(long double arg);
// erfcf - calculates the complementary error function instead of the error function itself.
float       erfcf(float arg);
double      erfc(double arg);
long double erfcl(long double arg);
// tgammaf - computes the gamma function of @arg.
float       tgammaf(float arg);
double      tgamma(double arg);
long double tgammal(long double arg);
// lgammaf - computes the natural logarithm of the absolute value of the gamma function of @arg.
float       lgammaf(float arg);
double      lgamma(double arg);
long double lgammal(long double arg);
// ceilf - computes the smallest integer value not less than @arg.
float       ceilf(float arg);
double      ceil(double arg);
long double ceill(long double arg);
// floorf - computes the largest integer value not greater than arg.
float       floorf(float arg);
double      floor(double arg);
long double floorl(long double arg);
// truncf - computes the nearest integer not greater in magnitude than @arg.
float       truncf(float arg);
double      trunc(double arg);
long double truncl(long double arg);
float       roundf(float arg);
double      round(double arg);
long double roundl(long double arg);
// lroundf - rounds to nearest integer, rounding away from zero in halfway cases
long      lroundf(float arg);
long      lround(double arg);
long      lroundl(long double arg);
long long llroundf(float arg);
long long llround(double arg);
long long llroundl(long double arg);
// nearbyintf - rounds the floating-point argument @arg to the nearest even integer value in floating-point format, using the current rounding mode, e.g. FE_UPWARD
float       nearbyintf(float arg);
double      nearbyint(double arg);
long double nearbyintl(long double arg);
// rintf - rounds away from zero (toward positive or negative infinity) in floating-point, using the current rounding mode
// infinity in the context of floating-point numbers represents a special value that indicates an overflow condition.
float rintf(float arg);
double rint(double arg);
long double rintl(long double arg);
long lrintf(float arg);
long lrint(double arg);
long lrintl(long double arg);
long long llrintf(float arg);
long long llrint(double arg);
long long llrintl(long double arg);
// frexpf - decomposes given floating point value x into a normalized fraction and an integral power of two.
float       frexpf(float arg, int *exp);
double      frexp(double arg, int *exp);
long double frexpl(long double arg, int *exp);
// ldexpf - multiplies a floating point value @arg by the number 2 raised to the @exp power.
float       ldexpf(float arg, int exp);
double      ldexp(double arg, int exp);
long double ldexpl(long double arg, int exp);
// modff - decomposes given floating point value arg into integral and fractional parts, each having the same type and sign as @arg.
// the integral part (in floating-point format) is stored in the object pointed to by iptr.
float       modff(float arg, float *iptr);
double      modf(double arg, double *iptr);
long double modfl(long double arg, long double *iptr);
// scalbnf - multiplies a floating point value @arg by FLT_RADIX raised to power @exp.
float       scalbnf(float arg, int exp);
double      scalbn(double arg, int exp);
long double scalbnl(long double arg, int exp);
float       scalblnf(float arg, long exp);
double      scalbln(double arg, long exp);
long double scalblnl(long double arg, long exp);
// nextafterf - first converts both arguments to the type of the function, then returns the next representable value of @from  in the direction of @to
// if @from equals @to, then @to is returned.
float       nextafterf(float from, float to);
double      nextafter(double from, double to);
long double nextafterl(long double from, long double to);
// nexttowardf - first converts the first argument to the type of the function, then returns the next representable value of @from in the direction of @to
// if @from equals @to, @to is returned, converted from long double to the return type of the function without loss of range or precision.
float       nexttowardf(float from, long double to);
double      nexttoward(double from, long double to);
long double nexttowardl(long double from, long double to);
// copysignf - composes a floating point value with the magnitude of @x and the sign of @y
float       copysignf(float x, float y);
double      copysign(double x, double y);
long double copysignl(long double x, long double y);
// fpclassify - categorizes floating point value @arg into the following categories: zero, subnormal, normal, infinite, NAN, or implementation-defined category. The macro returns an integral value.
#define fpclassify(arg)
// isfinite - determines if the given floating point number @arg has finite value i.e. it is normal, subnormal or zero, but not infinite or NaN. The macro returns an integral value.
#define isfinite(arg)
// isinf - determines if the given floating-point number @arg is positive or negative infinity. The macro returns an integral value.
#define isinf(arg)
// isnan - determines if the given floating point number @arg is a not-a-number (NaN) value. The macro returns an integral value.
#define isnan(arg)
// isnormal - determines if the given floating point number @arg is normal, i.e. is neither zero, subnormal, infinite, nor NaN. The macro returns an integral value.
#define isnormal(arg)
// signbit - determines if the given floating point number @arg is negative. The macro returns an integral value.
#define signbit(arg)
// isgreater - determines if the floating point number @x is greater than the floating-point number @y, without setting floating-point exceptions.
#define isgreater(x, y)
// isgreaterequal - determines if the floating point number @x is greater than or equal to the floating-point number @y, without setting floating-point exceptions.
#define isgreaterequal(x, y)
// isless - determines if the floating point number @x is less than the floating-point number @y, without setting floating-point exceptions.
#define isless(x, y)
// islessequal - determines if the floating point number @x is less than or equal to the floating-point number @y, without setting floating-point exceptions.
#define islessequal(x, y)
// islessgreater - determines if the floating point number @x is less than or greater than the floating-point number @y, without setting floating-point exceptions.
#define islessgreater(x, y)
// isunordered - determines if the floating point numbers @x and @y are unordered, that is, one or both are NaN and thus cannot be meaningfully compared with each other.
#define isunordered(x, y)
#define FP_FAST_FMA
#define FP_FAST_FMAF
#define FP_FAST_FMAL

#include <float.h>
// FLT_RADIX - the radix (integer base) used by the representation of all three floating-point types
#define FLT_RADIX
// DECIMAL_DIG - conversion from long double to decimal with at least DECIMAL_DIG digits and back to long double is the identity conversion:
// this is the decimal precision required to serialize/deserialize a long double
#define DECIMAL_DIG
#define FLT_DECIMAL_DIG
#define DBL_DECIMAL_DIG
#define LDBL_DECIMAL_DIG
// FLT_MIN - minimum, normalized, positive value of float, double and long double respectively
#define FLT_MIN
#define DBL_MIN
#define LDBL_MIN
// FLT_TRUE_MIN - minimum positive value of float, double and long double respectively
#define FLT_TRUE_MIN
#define DBL_TRUE_MIN
#define LDBL_TRUE_MIN
// FLT_MAX - maximum finite value of float, double and long double respectively
#define FLT_MAX
#define DBL_MAX
#define LDBL_MAX
// FLT_EPSILON - absolute value difference between 1.0 and the next representable value for float, double and long double respectively
#define FLT_EPSILON
#define DBL_EPSILON
#define LDBL_EPSILON
// FLT_DIG - number of decimal digits that are guaranteed to be preserved in text → float/double/long double → text roundtrip without change due to rounding or overflow
#define FLT_DIG
#define DBL_DIG
#define LDBL_DIG
// FLT_MANT_DIG - number of base-FLT_RADIX digits that are in the floating-point mantissa and that can be represented without losing precision for float, double and long double respectively
#define FLT_MANT_DIG
#define DBL_MANT_DIG
#define LDBL_MANT_DIG
// FLT_MIN_EXP - minimum negative integer such that FLT_RADIX raised by power one less than that integer is a normalized float, double and long double respectively
#define FLT_MIN_EXP
#define DBL_MIN_EXP
#define LDBL_MIN_EXP
// FLT_MIN_10_EXP - minimum negative integer such that 10 raised by power one less than that integer is a normalized float, double and long double respectively
#define FLT_MIN_10_EXP
#define DBL_MIN_10_EXP
#define LDBL_MIN_10_EXP
// FLT_MAX_EXP - maximum positive integer such that FLT_RADIX raised by power one less than that integer is a representable finite float, double and long double respectively
#define FLT_MAX_EXP
#define DBL_MAX_EXP
#define LDBL_MAX_EXP
// FLT_MAX_10_EXP - maximum positive integer such that 10 raised by power one less than that integer is a representable finite float, double and long double respectively
#define FLT_MAX_10_EXP
#define DBL_MAX_10_EXP
#define LDBL_MAX_10_EXP
// FLT_ROUNDS - rounding mode of floating-point arithmetic
#define FLT_ROUNDS
// FLT_EVAL_METHOD - use of extended precision for intermediate results: 0 = not used, 1 = double is used instead of float, 2 = long double is used
#define FLT_EVAL_METHOD
// FLT_HAS_SUBNORM - whether the type supports subnormal (denormal) numbers: -1 = indeterminable, 0 = absent, 1 = present
#define FLT_HAS_SUBNORM
// DBL_HAS_SUBNORM - subset of denormalized numbers that fill the underflow gap around zero in floating-point arithmetic.
#define DBL_HAS_SUBNORM
// LDBL_HAS_SUBNOR - any non-zero number with magnitude smaller than the smallest positive normal number is subnormal,
// while denormal can also refer to numbers outside that range
#define LDBL_HAS_SUBNOR

#include <fenv.h>
// @fenv_t - the type representing the entire floating-point environment
typedef fenv_t
// @fexcept_t - the type representing all floating-point status flags collectively
typedef fexcept_t
// feclearexcept - attempts to clear the floating-point exceptions that are listed in the bitmask argument @excepts, which is a bitwise OR of the floating-point exception macros.
int feclearexcept(int excepts);
// fetestexcept - determines which of the specified subset of the floating-point exceptions are currently set. The argument @excepts is a bitwise OR of the floating-point exception macros.
int fetestexcept(int excepts);
// feraiseexcept - attempts to raise all floating-point exceptions listed in @excepts (a bitwise OR of the floating-point exception macros).
// if one of the exceptions is FE_OVERFLOW or FE_UNDERFLOW, this function may additionally raise FE_INEXACT.
int feraiseexcept(int excepts);
// fegetexceptflag - attempts to obtain the full contents of the floating-point exception flags that are listed in the bitmask argument @excepts,
// which is a bitwise OR of the floating-point exception macros.
int fegetexceptflag(fexcept_t *flagp, int excepts);
// fesetexceptflag - attempts to copy the full contents of the floating-point exception flags that are listed in @excepts from @flagp
// into the floating-point environment. does not raise any exceptions, only modifies the flags.
int fesetexceptflag(const fexcept_t *flagp, int excepts);
// fesetround - attempts to establish the floating-point rounding direction equal to the argument @round, which is expected to be one of the floating-point rounding macros.
int fesetround(int round);
// fegetround - returns the value of the floating-point rounding macro that corresponds to the current rounding direction.
int fegetround();
// fegetenv - attempts to store the status of the floating-point environment in the object pointed to by @envp.
int fegetenv(fenv_t *envp)
// fesetenv - attempts to establish the floating-point environment from the object pointed to by envp.
// the value of that object must be previously obtained by a call to feholdexcept or fegetenv or be a floating-point macro constant.
// if any of the floating-point status flags are set in @envp, they become set in the environment (and are then testable with fetestexcept),
// but the corresponding floating-point exceptions are not raised (execution continues uninterrupted)
int fesetenv(const fenv_t *envp);
// feholdexcept - first, saves the current floating-point environment to the object pointed to by @envp (similar to fegetenv),
// then clears all floating-point status flags, and then installs the non-stop mode: future floating-point exceptions
// will not interrupt execution (will not trap), until the floating-point environment is restored by feupdateenv or fesetenv.
// this function may be used in the beginning of a subroutine that must hide the floating-point exceptions it may raise from the caller.
// if only some exceptions must be suppressed, while others must be reported, the non-stop mode is usually ended with a call to feupdateenv
// after clearing the unwanted exceptions.
int feholdexcept(fenv_t *envp);
// feupdateenv - first, remembers the currently raised floating-point exceptions, then restores the floating-point environment from the object
// pointed to by @envp (similar to fesetenv), then raises the floating-point exceptions that were saved.
// this function may be used to end the non-stop mode established by an earlier call to feholdexcept.
int feupdateenv(const fenv_t *envp);
// FE_DIVBYZERO - all these macro constants (except FE_ALL_EXCEPT) expand to integer constant expressions that are distinct powers of 2,
#define FE_DIVBYZERO
// FE_INEXACT - which uniquely identify all supported floating-point exceptions. Each macro is only defined if it is supported.
#define FE_INEXACT
// FE_INVALID - the macro constant FE_ALL_EXCEPT, which expands to the bitwise OR of all other FE_*, is always defined and is
#define FE_INVALID
// FE_OVERFLOW - zero if floating-point exceptions are not supported by the implementation.
#define FE_OVERFLOW
#define FE_UNDERFLOW
#define FE_ALL_EXCEPT  FE_DIVBYZERO | FE_INEXACT | \
                       FE_INVALID | FE_OVERFLOW |  \
                       FE_UNDERFLOW
#define FE_DOWNWARD
#define FE_TONEAREST
#define FE_TOWARDZERO
#define FE_UPWARD
#define FE_DFL_ENV

#include <ctype.h>
int isalnum(int ch);
int isalpha(int ch);
int islower(int ch);
int isupper(int ch);
int isdigit(int ch);
int isxdigit(int ch);
int iscntrl(int ch);
int isgraph(int ch);
int isspace(int ch);
int isblank(int ch);
int isprint(int ch);
int ispunct(int ch);
int tolower(int ch);
int toupper(int ch);

#include <errno.h>
#define MACRO_NAME VALUE
#define errno           // Thread-local error variable (Implementation-defined)
#define E2BIG           // Argument list too long.
#define EACCES          // Permission denied.
#define EADDRINUSE      // Address already in use.
#define EADDRNOTAVAIL   // Address not available.
#define EAFNOSUPPORT    // Address family not supported.
#define EAGAIN          // Resource temporarily unavailable (may be the same value as EWOULDBLOCK)
#define EALREADY        // Connection already in progress.
#define EBADE           // Invalid exchange.
#define EBADF           // Bad file descriptor.
#define EBADFD          // File descriptor in bad state.
#define EBADMSG         // Bad message.
#define EBADR           // Invalid request descriptor.
#define EBADRQC         // Invalid request code.
#define EBADSLT         // Invalid slot.
#define EBUSY           // Device or resource busy.
#define ECANCELED       // Operation canceled.
#define ECHILD          // No child processes.
#define ECHRNG          // Channel number out of range.
#define ECOMM           // Communication error on send.
#define ECONNABORTED    // Connection aborted.
#define ECONNREFUSED    // Connection refused.
#define ECONNRESET      // Connection reset.
#define EDEADLK         // Resource deadlock avoided.
#define EDESTADDRREQ    // Destination address required.
#define EDOM            // Mathematics argument out of domain of function.
#define EDQUOT          // Disk quota exceeded.
#define EEXIST          // File exists.
#define EFAULT          // Bad address.
#define EFBIG           // File too large.
#define EHOSTDOWN       // Host is down.
#define EHOSTUNREACH    // Host is unreachable.
#define EHWPOISON       // Memory page has hardware error.
#define EIDRM           // Identifier removed.
#define EILSEQ          // POSIX.1: illegal byte sequence, glibc: invalid or incomplete multibyte or wide character
#define EINPROGRESS     // Operation in progress.
#define EINTR           // Interrupted function call.
#define EINVAL          // Invalid argument.
#define EIO             // Input/output error.
#define EISCONN         // Socket is connected.
#define EISDIR          // Is a directory.
#define EISNAM          // Is a named type file.
#define EKEYEXPIRED     // Key has expired.
#define ERANGE          // Result too large.
#define EKEYREJECTED    // Key was rejected by service.
#define EKEYREVOKED     // Key has been revoked.
#define EL2HLT          // Level 2 halted.
#define EL2NSYNC        // Level 2 not synchronized.
#define EL3HLT          // Level 3 halted.
#define EL3RST          // Level 3 reset.
#define ELIBACC         // Cannot access a needed shared library.
#define ELIBBAD         // Accessing a corrupted shared library.
#define ELIBMAX         // Attempting to link in too many shared libraries.
#define ELIBSCN         // .lib section in a.out corrupted
#define ELIBEXEC        // Cannot exec a shared library directly.
#define ELNRNG          // Link number out of range.
#define ELOOP           // Too many levels of symbolic links.
#define EMEDIUMTYPE     // Wrong medium type.
#define EMFILE          // Too many open files. Commonly caused by exceeding the RLIMIT_NOFILE resource limit described in getrlimit(2)
                        // Can also be caused by exceeding the limit specified in /proc/sys/fs/nr_open.
#define EMLINK          // Too many links.
#define EMSGSIZE        // Message too long.
#define EMULTIHOP       // Multihop attempted.
#define ENAMETOOLONG    // Filename too long.
#define ENETDOWN        // Network is down.
#define ENETRESET       // Connection aborted by network.
#define ENETUNREACH     // Network unreachable.
#define ENFILE          // Too many open files in system. On Linux, this is probably a result of encountering the /proc/sys/fs/file-max limit (see proc(5))
#define ENOANO          // No anode.
#define ENOBUFS         // No buffer space available (XSI STREAMS option)
#define ENODATA         // The named attribute does not exist, or the process has no access to this attribute; see xattr(7). 
                        // (XSI STREAMS option), this error was described as "No message is available on the STREAM head read queue".
#define ENODEV          // No such device.
#define ENOENT          // No such file or directory. Typically, this error results when a specified pathname does not exist, or one of the components in
                        // the directory prefix of a pathname does not exist, or the specified pathname is a dangling symbolic link.
#define ENOEXEC         // Exec format error.
#define ENOKEY          // Required key not available.
#define ENOLCK          // No locks available.
#define ENOLINK         // Link has been severed.
#define ENOMEDIUM       // No medium found.
#define ENOMEM          // Not enough space/cannot allocate memory.
#define ENOMSG          // No message of the desired type.
#define ENONET          // Machine is not on the network.
#define ENOPKG          // Package not installed.
#define ENOPROTOOPT     // Protocol not available.
#define ENOSPC          // No space left on device.
#define ENOSR           // No STREAM resources (XSI STREAMS option)
#define ENOSTR          // Not a STREAM (XSI STREAMS option)
#define ENOSYS          // Function not implemented.
#define ENOTBLK         // Block device required.
#define ENOTCONN        // The socket is not connected.
#define ENOTDIR         // Not a directory.
#define ENOTEMPTY       // Directory not empty.
#define ENOTRECOVERABLE // State not recoverable.
#define ENOTSOCK        // Not a socket.
#define ENOTSUP         // Operation not supported.
#define ENOTTY          // Inappropriate I/O control operation.
#define ENOTUNIQ        // Name not unique on network.
#define ENXIO           // No such device or address.
#define EOPNOTSUPP      // Operation not supported on socket (ENOTSUP and EOPNOTSUPP have the same value on Linux, but according to POSIX.1 these error values should be distinct.)
#define EOVERFLOW       // Value too large to be stored in data type.
#define EOWNERDEAD      // Owner died.
#define EPERM           // Operation not permitted.
#define EPFNOSUPPORT    // Protocol family not supported.
#define EPIPE           // Broken pipe.
#define EPROTO          // Protocol error.
#define EPROTONOSUPPORT // Protocol not supported.
#define EPROTOTYPE      // Protocol wrong type for socket.
#define EREMCHG         // Remote address changed.
#define EREMOTE         // Object is remote.
#define EREMOTEIO       // Remote I/O error.
#define ERESTART        // Interrupted system call should be restarted.
#define ERFKILL         // Operation not possible due to RF-kill.
#define EROFS           // Read-only filesystem.
#define ESHUTDOWN       // Cannot send after transport endpoint shutdown.
#define ESPIPE          // Invalid seek.
#define ESOCKTNOSUPPORT // Socket type not supported.
#define ESRCH           // No such process.
#define ESTALE          // Stale file handle. This error can occur for NFS and for other filesystems.
#define ESTRPIPE        // Streams pipe error.
#define ETIME           // Timer expired (XSI STREAMS option, POSIX.1 says "STREAM ioctl(2) timeout".)
#define ETIMEDOUT       // Connection timed out.
#define ETOOMANYREFS    // Too many references: cannot splice.
#define ETXTBSY         // Text file busy.
#define EUCLEAN         // Structure (filesystem object) needs cleaning.
#define EUNATCH         // Protocol driver not attached.
#define EUSERS          // Too many users.
#define EWOULDBLOCK     // Operation would block (may be same value as EAGAIN)
#define EXDEV           // Invalid cross-device link.
#define EXFULL          // Exchange full.

#include <time.h>
typedef clock_t type-name; // processor time since era type 
// CLOCKS_PER_SEC - expands to an expression (not necessarily a compile-time constant) of type clock_t equal to the number of clock ticks per second, as returned by clock().
// POSIX defines CLOCKS_PER_SEC as one million, regardless of the actual precision of clock.
#define CLOCKS_PER_SEC
// @time_t - real arithmetic type capable of representing times. Although not defined by the C standard, this is almost always an integral value holding the number of seconds
// (not counting leap seconds) since 00:00, Jan 1 1970 UTC, corresponding to POSIX time.
typedef time_t type-name;
struct timespec; // time in seconds and nanoseconds
struct tm; // calendar time type
typedef time_t type-name; // calendar time since epoch type 
// time - returns the current calendar time encoded as a time_t object, and also stores it in the time_t object pointed to by @arg (unless arg is a null pointer)
time_t time(time_t *arg);
// difftime - computes difference between two calendar times as time_t objects (time_end - time_beg) in seconds.
// If @time_end refers to time point before @time_beg then the result is negative.
double difftime(time_t time_end, time_t time_beg);
// clock - returns the approximate processor time used by the process since the beginning of an implementation-defined era related to the program's execution.
// To convert result value to seconds, divide it by CLOCKS_PER_SEC. Only the difference between two values returned by different calls to clock is meaningful,
// as the beginning of the clock era does not have to coincide with the start of the program. clock time may advance faster or slower than the wall clock,
// depending on the execution resources given to the program by the operating system. For example, if the CPU is shared by other processes, clock time may
// advance slower than wall clock. On the other hand, if the current process is multithreaded and more than one execution core is available, clock time may advance faster than wall clock.
clock_t clock(void);
// timespec_get - modifies the timespec object pointed to by @ts to hold the current calendar time in the time base @base.
int timespec_get(struct timespec *ts, int base);
// timespec_getres - if @ts is non-null and base is supported by timespec_get, modifies @ts to hold the resolution of time provided by timespec_get for @base.
// For each supported base, multiple calls to timespec_getres during the same program execution have identical results.
int timespec_getres(struct timespec *ts, int base);
// TIME_UTC - expands to a value suitable for use as the base argument of timespec_get
#define TIME_UTC
// asctime - converts given calendar time tm to a textual representation of the following fixed 25-character form: Www Mmm dd hh:mm:ss yyyy
[[deprecated]] char *asctime(const struct tm *time_ptr);
// asctime_s - same as asctime except message is written into user-provided storage @buf guaranteed to be null-terminated
errno_t asctime_s(char *buf, rsize_t bufsz, const struct tm *time_ptr);
// ctime - converts given time since epoch to a calendar local time and then to a textual representation, as if by calling asctime(localtime(timer))
// or asctime(localtime_r(timer, &(struct tm){0}))(since C23). This function is deprecated and should not be used in new code.
[[deprecated]] char *ctime(const time_t *timer);
errno_t ctime_s(char *buf, rsize_t bufsz, const time_t *timer);
// strftime - converts the date and time information from a given calendar time tp to a
// null-terminated multibyte character string @str according to format string format. Up to count bytes are written.
size_t strftime(char *restrict str, size_t count, const char *restrict format, const struct tm *restrict tp);
// gmtime - converts given time since epoch (a time_t value pointed to by @timer) into calendar time, expressed in Coordinated Universal Time (UTC)
// in the struct tm format. The result is stored in static storage and a pointer to that static storage is returned.
struct tm *gmtime(const time_t *timer);
struct tm *gmtime_r(const time_t *timer, struct tm *buf);
struct tm *gmtime_s(const time_t *restrict timer, struct tm *restrict buf);
// localtime - converts given time since epoch (a time_t value pointed to by @timer) into calendar time, expressed in local time, in the struct tm format.
// The result is stored in static storage and a pointer to that static storage is returned.
struct tm *localtime(const time_t *timer);
struct tm *localtime_r(const time_t *timer, struct tm *buf);
struct tm *localtime_s(const time_t *restrict timer, struct tm *restrict buf);
// mktime - renormalizes local calendar time expressed as a struct tm object and also converts it to time since epoch as a time_t object.
// @arg->tm_wday and @arg->tm_yday are ignored. The values in @arg are not checked for being out of range.
time_t mktime(struct tm *arg);
int clock_getres(clockid_t clock_id, struct timespec *res);
int clock_gettime(clockid_t clock_id, struct timespec *tp);
int clock_settime(clockid_t clock_id, const struct timespec *tp);

