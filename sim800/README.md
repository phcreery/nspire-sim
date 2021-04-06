First install ndless-sdk and nspire-io


```
export PATH="/mnt/c/Users/phcre/Documents/TI-Nspire/SDK/Ndless/ndless-sdk/toolchain/install/bin:/mnt/c/Users/phcre/Documents/TI-Nspire/SDK/Ndless/ndless-sdk/bin:${PATH}"
make clean
make
```


Resources:
https://hackspire.org/index.php/C_and_assembly_development_introduction
https://tiplanet.org/forum/viewtopic.php?t=18130&p=198701&lang=en
https://github.com/ndless-nspire/Ndless/tree/master/ndless-sdk/samples
https://github.com/compujuckel/nspire-io
https://github.com/TheStaticTurtle/nspire-communication
https://blog.thestaticturtle.fr/ti-nspire-sms-shield/


```
int main(void)
{
   assert_ndless_rev(874);
   // clrscr();
   
   nio_console csl;
   nio_init(&csl,NIO_MAX_COLS,NIO_MAX_ROWS,0,0,NIO_COLOR_WHITE,NIO_COLOR_BLACK,TRUE);
   nio_set_default(&csl);
   // nio_color(&csl,NIO_COLOR_BLACK,NIO_COLOR_WHITE); // inverted
   // nio_puts("Nspire I/O testing tool\n");
   nio_color(&csl,NIO_COLOR_WHITE,NIO_COLOR_BLACK); // black on white
   nio_printf("Compiled %s %s\n",__DATE__,__TIME__);
   nio_puts("Press any key to flush console...\n");
   // nio_drawing_enabled(&csl,FALSE);
   // nio_putchar('A');
   // nio_fputc('a',&csl);
   // nio_putchar('\n');
   // nio_puts("Color palette test:\n");
   // int i, j;
   // for(i = 0; i < 16; i++)
   // {
   //    for(j = 0; j < 16; j++)
   //    {
   //       nio_color(&csl,i,j);
   //       nio_putchar('X');
   //    }
   //    nio_putchar('\n');
   // }
   // nio_color(&csl,NIO_COLOR_WHITE,NIO_COLOR_BLACK);
   // nio_puts("This line is too long for the Nspire's screen so we have to continue it on the next line...\n");
   // wait_key_pressed();
   // nio_fflush(&csl);
   // nio_drawing_enabled(&csl,TRUE);
   // nio_puts("Enter text:\n");
   // char input[100] = {0};
   // nio_getsn(input,100);
   // nio_printf("Your text was:\n%s\n",input);
//    nio_puts("Enter a number: ");
//    int num;



   // nio_puts("Testing UART...\n");
   // uart_puts("Nspire I/O testing tool\n");
   // uart_printf("Compiled %s %s\n",__DATE__,__TIME__);
   
   // uart_puts("Enter text: ");
   // uart_getsn(input,100);
   // uart_printf("Your text was: %s\n",input);
   

   

   nio_puts("Press any ESC to exit and CTRL to send msg...\n");
	while(!isKeyPressed(KEY_NSPIRE_ESC)){
		if(isKeyPressed(KEY_NSPIRE_CTRL) && key_already_pressed == 0){
			nio_printf(">");
			char input2[100] = {0};
			nio_getsn(input2,100);
			uart_printf("\r\n");
			uart_printf(input2);
			key_already_pressed = 1;
		}
		if(!any_key_pressed())
			key_already_pressed = 0;

		// if(uart_ready()) {
      //    nio_puts("UART ready...");
		// 	char input[100] = {0};
		// 	uart_getsn(input,100);
      // //   nio_puts(input);
		// 	if(strcmp(oldinput,input) != 0) {
		// 		if(input != voidlist) {
		// 			nio_puts(input);
		// 			strcpy(oldinput,input);
		// 			strcpy(input,voidlist);
		// 		}
		// 	}
		// }
      if(uart_ready()) {
			char input[1024] = {0};
			uart_getsn_mdf(input,1024);
			nio_puts(input);
		}
	}



   nio_puts("Tests finished.");
   nio_free(&csl);
   wait_key_pressed();
   
   return 0;
}
'''
