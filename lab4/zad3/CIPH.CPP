/* PROGRAM CIPHER  */
/* Program ciphering one block of text.
The values of hexinbin are read from the file hex.dat.
The procedure GLOPEN is used to assign hex.dat to infile
and open the file for reading. */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>

typedef int gl32array[32];
typedef int gl48array[48];
typedef int gl56array[56];
typedef int gl64array[64];
typedef int gl16_4_8array[16][4][8];
typedef int gl4_16array[4][16];

/*   gl32array    ipp;
   gl48array    ipc2, iet;
   gl56array    glicd, ipc1;
   gl64array    ip, ipm, inpt, key, jotput;
   int      	is[16][4][8];
   int      	ibin[4][16];
   int      	hexinbin[4][16];
   int      	i, j, inte, weight;
   int      	desflg, ksflg, cyflg;
   char         htext[17], hkey[17];
   char         oper;
   FILE         *infile;
*/

void glopen(FILE **infile, const char *filename) // zmiana z char *  na const char *
{
   if ((*infile = fopen(filename, "r")) == NULL)
   {
      printf("File %s can't be opened", filename);
      exit(1);
   }
}

void ks(gl64array key,
        int n,
        gl48array kn,
        int ksflg,
        gl56array ipc1,
        gl48array ipc2,
        gl56array glicd)
/* The values of ipc1 and ipc2 are read from the file ksinpu.dat.
   The procedure GLOPEN assigns ksinpu.dat to infile
   and opens the file for reading. */
{
   int i, j, it, id, ic;
   FILE *infile;

   if (ksflg)
   {
      ksflg = 0;
      glopen(&infile, "KSINPU.DAT");
      for (i = 0; i <= 55; i++)
         fscanf(infile, "%d", &(ipc1[i]));
      for (i = 0; i <= 47; i++)
         fscanf(infile, "%d", &(ipc2[i]));
      fclose(infile);
   }

   if ((n == 0))
   {
      for (j = 0; j <= 55; j++)
      {
         glicd[j] = key[ipc1[j] - 1];
      }
   }

   it = 1;
   if (((n == 0) || (n == 1) || (n == 8) || (n == 15)))
      it = 0;

   for (i = 0; i <= it; i++)
   {
      ic = glicd[0];
      id = glicd[28];
      for (j = 0; j <= 26; j++)
      {
         glicd[j] = glicd[j + 1];
         glicd[j + 28] = glicd[j + 29];
      }
      glicd[27] = ic;
      glicd[55] = id;
   }

   for (j = 0; j <= 47; j++)
      kn[j] = glicd[ipc2[j] - 1];
}

void cyfun(gl32array ir,
           gl48array k,
           gl32array iout,
           int cyflg,
           gl48array iet,
           gl32array ipp,
           gl16_4_8array is,
           gl4_16array ibin)
/* Programs using routine CYFUN must define the types:
  TYPE
     gl32array = ARRAY [1..32] OF int;
     gl48array = ARRAY [1..48] OF int;
  in the main routine. They must also declare the variables:
  VAR
     iet: gl48array;
     ipp: gl32array;
     is: ARRAY[1..16,1..4,1..8] OF int;
     ibin: ARRAY[1..4,1..16] OF int;
     cyflg: boolean;
  and initialize cyflg to true. The values of iet, ipp, is and ibin are read
  from the file cyfuni.dat. The procedure GLOPEN assigns cyfuni.dat to
  infile and opens the file for reading. */
{
   int j, jj, irow, icol, kk, iss, ki;
   gl48array ie;
   gl32array itmp;
   FILE *infile;

   if (cyflg)
   {
      cyflg = 0;
      glopen(&infile, "CYFUNI.DAT");
      for (j = 0; j <= 47; j++)
         fscanf(infile, "%d", &(iet[j]));
      for (j = 0; j <= 31; j++)
         fscanf(infile, "%d", &(ipp[j]));
      for (jj = 0; jj <= 7; jj++)
         for (ki = 0; ki <= 3; ki++)
            for (j = 0; j <= 15; j++)
               fscanf(infile, "%d", &(is[j][ki][jj]));
      for (j = 0; j <= 15; j++)
         for (ki = 0; ki <= 3; ki++)
            fscanf(infile, "%d", &(ibin[ki][j]));
      fclose(infile);
   }

   for (j = 0; j <= 47; j++)
      ie[j] = (((ir[iet[j] - 1] + k[j]) % 2) + 2) % 2;

   for (jj = 0; jj <= 7; jj++)
   {
      j = 6 * jj;
      irow = 2 * ie[j] + ie[j + 5] - 1;
      icol = 8 * ie[j + 1] + 4 * ie[j + 2] + 2 * ie[j + 3] + ie[j + 4] - 1;
      iss = is[icol + 1][irow + 1][jj] - 1;
      kk = 4 * (jj);
      for (ki = 0; ki <= 3; ki++)
         itmp[kk + ki] = ibin[ki][iss + 1];
   }

   for (j = 0; j <= 31; j++)
      iout[j] = itmp[ipp[j] - 1];
}

void des(gl64array inpt,
         gl64array key,
         gl64array jotput,
         int desflg,
         gl64array ip,
         gl64array ipm,
         int ksflg,
         gl56array ipc1,
         gl48array ipc2,
         gl56array glicd,
         int cyflg,
         gl48array iet,
         gl32array ipp,
         gl16_4_8array is,
         gl4_16array ibin)

/* The values of ip and ipm are read from the file desinp.dat.
   The procedure GLOPEN is used to assign desinp.dat
   to infile and open the file for reading. */
{
   int j, ii, ic, i;
   gl32array titmp, icf;
   gl64array itmp;
   int kns[48][16];
   gl48array tkns;
   FILE *infile;

   if (desflg)
   {
      desflg = 0;
      glopen(&infile, "DESINP.DAT");
      for (i = 0; i <= 63; i++)
         fscanf(infile, "%d", &(ip[i]));
      for (i = 0; i <= 63; i++)
         fscanf(infile, "%d", &(ipm[i]));
      fclose(infile);
   }

   for (i = 0; i <= 15; i++)
   {
      ks(key, i, tkns, ksflg, ipc1, ipc2, glicd);
      for (j = 0; j <= 47; j++)
         kns[j][i] = tkns[j];
   }

   for (j = 0; j <= 63; j++)
      itmp[j] = inpt[ip[j] - 1];

   for (i = 0; i <= 15; i++)
   {
      ii = i;
      for (j = 0; j <= 47; j++)
         tkns[j] = kns[j][ii];

      for (j = 0; j <= 31; j++)
         titmp[j] = itmp[32 + j];

      cyfun(titmp, tkns, icf, cyflg, iet, ipp, is, ibin);
      for (j = 0; j <= 31; j++)
      {
         ic = icf[j] + itmp[j];
         itmp[j] = itmp[j + 32];
         itmp[j + 32] = abs(ic % 2);
      }
   }

   for (j = 0; j <= 31; j++)
   {
      ic = itmp[j];
      itmp[j] = itmp[j + 32];
      itmp[j + 32] = ic;
   }

   for (j = 0; j <= 63; j++)
      jotput[j] = itmp[ipm[j] - 1];
}

void HEXTOBIN(char *htext,
              gl64array result,
              gl4_16array hexinbin)
/* Converts hexadecimal digits to binary form */
{
   int i, j, inte;

   for (i = 0; i <= 15; i++)
   { /* Convertion hexadecimal to int. */
      if ((htext[i] >= '0') && (htext[i] <= '9'))
         inte = int(0) + htext[i] - '0';
      else if ((htext[i] >= 'a') && (htext[i] <= 'f'))
         inte = int(0) + htext[i] - 'a' + 10;
      else
         inte = 0;
      /* Convertion int to binary. */
      for (j = 0; j <= 3; j++)
         result[i * 4 + j] = hexinbin[j][inte];
   }
}

void BINTOHEX(gl64array jotput)
/* Converts binary number to printed hexadecimal equivalent */
{
   int i, j, inte, weight;

   printf("Cipher hex: ");
   for (i = 0; i <= 15; i++)
   {
      inte = 0;
      weight = 1; /* Convertion binary to int. */
      for (j = 0; j <= 3; j++)
      {
         inte = inte + jotput[(i * 4 + 3 - j)] * weight;
         weight = weight * 2;
      } /* Convertion intger to hexadecimal. */
      if (inte <= 9)
         printf("%c   ", char(inte) + '0');
      else
         printf("%c   ", char(inte) - 10 + 'a');
   }
}

void TEXTTOHEX(std::string inputText, std::string &outputText)
{
   std::stringstream ss;
   for (const auto &item : inputText)
   {
      ss << std::hex << int(item);
   }

   std::cout << std::endl;

   outputText = ss.str();
}

int main()
{ /* Main program */
   FILE *infile;
   int i, j;
   int desflg, ksflg, cyflg;
   gl32array ipp;
   gl48array ipc2, iet;
   gl56array glicd, ipc1;
   gl64array ip, ipm, inpt, key, jotput;
   gl16_4_8array is;
   gl4_16array ibin;
   gl4_16array hexinbin;
   char htext[17], hkey[17];
   int a;

   system("cls"); // clrscr();
   glopen(&infile, "HEX.DAT");
   for (i = 0; i <= 15; i++)
      for (j = 0; j <= 3; j++)
         fscanf(infile, "%d", &(hexinbin[j][i]));

   fclose(infile);
   printf("\n");
   printf("*** PROGRAM CIPHER ***\n");
   printf("\n");

   std::string inputText;
   std::string outputText;
   printf("Type plaintext <8 ASCII characters + ENTER>:\n");
   printf("Type ascii:\n");
   printf("00000000\r");
   std::cin >> inputText;
   TEXTTOHEX(inputText, outputText);

   for (int i = 0; i < 16; i++)
   {
      htext[i] = outputText.at(i);
   }
   htext[16] = 0;

   std::cout << "Text hex:   " << htext << std::endl;
   HEXTOBIN(htext, inpt, hexinbin);
   printf("Text bin:   ");

   for (i = 0; i <= 63; i++)
      printf("%d", inpt[i]);
   printf("\n");
   printf("\n");

   printf("Type Key <8 ASCII characters + ENTER>:\n");
   printf("Key ascii:\n");
   printf("00000000\r");
   std::cin >> inputText;
   TEXTTOHEX(inputText, outputText);

   for (int i = 0; i < 16; i++)
   {
      hkey[i] = outputText.at(i);
   }
   hkey[16] = 0;

   hkey[16] = 0;
   std::cout << "Key hex:    " << hkey << std::endl;

   HEXTOBIN(hkey, key, hexinbin);
   printf("Key bin:    ");
   for (i = 0; i <= 63; i++)
      printf("%d", key[i]);
   printf("\n");
   printf("\n");

   ksflg = 1;
   cyflg = 1;
   desflg = 1;
   printf("Ciphering is running...");
   des(inpt, key, jotput, desflg, ip, ipm, ksflg, ipc1, ipc2, glicd, cyflg, iet, ipp, is, ibin);
   printf("Done.\n");
   printf("\n");
   BINTOHEX(jotput);
   printf("\n");
   printf("Cipher bin: ");
   for (i = 0; i <= 63; i++)
      printf("%d", jotput[i]);
   printf("\n");
   printf("\n");
   printf("Press ENTER\n");
   getch();
}
