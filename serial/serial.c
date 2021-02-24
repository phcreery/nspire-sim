#include <os.h>
#include <nspireio/nspireio.h>
// #include <nspireio/uart.hpp>
// #include <nspireio/console.hpp>

// #define emulate_serial

// int key_already_pressed = 0;
char oldinput[100] = {0};
char voidlist[100] = {0};
int dispInput = 0;

#ifndef emulate_serial

// Function added with && on every loop to prevent the programm of getting stuck
int releaseFunc()
{
   if (isKeyPressed(KEY_NSPIRE_R) || isKeyPressed(KEY_NSPIRE_ESC))
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

char uart_getchar_mdf(void)
{
   volatile unsigned *recv_buffer_reg = (unsigned *)0x90020000;
   while (!uart_ready() && releaseFunc())
      idle();
   char tmp = *recv_buffer_reg;
   // uart_putchar(tmp);
   return tmp;
}

char uart_getchar_mdf_test(void)
{
   // volatile unsigned *recv_buffer_reg = (unsigned *)0x90020000;
   // while (!uart_ready())
   //    idle();
   // char tmp = *recv_buffer_reg;
   char tmp = 'A' + (random() % 26);
   // uart_putchar(tmp);
   return tmp;
}

int cprint_ascii(char input)
{
   // Print Raw ASCII Values
   char snum[5];
   if ((int)input != 13 && (int)input != 10)
   {
      itoa((int)input, snum, 10);
      nio_puts(snum);
   }
   return 1;
}

// Modidification of the uart_getsn() function to remove the echo. And the 32 FIFO Limit
char *uart_getsn_mdf(char *str, int num)
{
   int i = 0;
   int max = num - 1;

   while (1 && releaseFunc())
   {
      while (uart_ready() && releaseFunc()) // while (uart_ready() && releaseFunc())
      {
         // nio_puts("UART ready...");
         if (i == max)
         {
            str[i] = 0;
            return str;
         }
         // char c = uart_getchar();
         char c = uart_getchar_mdf();
         // cprint_ascii(c);
         str[i] = c;
         if (c == '\b')
         {
            i -= 2;
         }
         else if (c == 0x0A) // '\r' // cursor back to start
         {
            str[i] = '_';
            // str[i] = 0;
            // uart_putchar('\n');
            // return str; // Do not return?
         }
         else if (c == 0x0d) // '\n' // newline
         {
            str[i] = '_';
            // str[i] = 0;
            // uart_putchar('\n');
            // return str; // Do not return?
         }

         if ((strstr(str, "OK") != NULL) || (strstr(str, "ERROR") != NULL))
         {
            return str;
         }

         i++;
      }
   }
   return str;
}

// Modidification of the uart_getsn() function to remove the echo. And the 32 FIFO Limit
char *uart_getsn_mdf_test(char *str, int num)
{
   int i = 0;
   int max = num - 1;
   while (1 && releaseFunc())
   {
      while (1 && releaseFunc())
      {
         // nio_puts("UART ready...");
         if (i == max)
         {
            str[i] = 0;
            return str;
         }
         // char c = uart_getchar();
         char c = uart_getchar_mdf_test();
         cprint_ascii(c);
         str[i] = c;
         if (c == '\b')
         {
            // i -= 2;
         }
         else if (c == '\r') // was \r but \n works better i think??
         {
            str[i] = 0;
            return str;
         }
         i++;
      }
   }
   return str;
}

char *uart_getsn_mdf2(char *str, int num)
{
   int i;
   for (i = 0; i < num - 1; i++)
   {
      // char c = uart_getchar();
      char c = uart_getchar_mdf();
      cprint_ascii(c);
      str[i] = c;
      if (c == '\b')
      {
         // uart_puts(" \b");
         i -= 2;
      }
      else if (c == '\r')
      {
         str[i] = 0;
         uart_putchar('\n');
         return str;
      }
   }
   str[num] = 0;
   return str;
}

// Return the number of char in a car array
int numberOfCharsInArray(char *array)
{
   return strlen(array);
}

// Send UART Command
void serial_send(char *str)
{
   uart_printf(str);
   // uart_puts(str);
   // uart_printf("\n\r");
   uart_printf("\r");
}

// Get UART Data (non-blocking)
char *serial_rec(char *str)
{
   if (uart_ready())
   {
      uart_getsn_mdf(str, 1024);
   }
   return str;
}

// Get UART Data (non-blocking)
char *await_serial_rec(char *str)
{

   while (!uart_ready() && releaseFunc())
   {
   }
   uart_getsn_mdf(str, 1024);
   return str;
}

// Get UART Data (non-blocking)
char *serial_request(char *command, char *str)
{

   serial_send(command);

   while (!uart_ready() && releaseFunc())
   {
   }

   uart_getsn_mdf(str, 1024);
   return str;
}

#else

void serial_send(char *str)
{
   uart_printf("\r\n");
   uart_printf(str);
}

char *serial_rec(char *str)
{
   // char str[1024] = {0};
   // char *str = malloc(sizeof(char) * 1024);
   // char *str = malloc(1024);
   *str = '\0'; // fill with NULLs
   strcpy(str, "emulated-message\r\n");
   return str;
}

#endif