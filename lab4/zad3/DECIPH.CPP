/* PROGRAM DECIPHER  */
/* Program deciphering one block of text.
The values of hexinbin are read from the file HEX.DAT.
The procedure GLOPEN is used to assign HEX.DAT to infile
and open the file for reading. */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <sstream>
#include <fstream>

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

void glopen(FILE **infile,
            const char *filename) // zmiana char * na const char*
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
/* The values of ipc1 and ipc2 are read from the file KSINPU.DAT.
   The procedure GLOPEN assigns KSINPU.DAT to infile
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
  from the file CYFUNI.DAT. The procedure GLOPEN assigns CYFUNI.DAT to
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

void d_des(gl64array inpt,
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

/* The values of ip and ipm are read from the file DESINP.DAT.
   The procedure GLOPEN is used to assign DESINP.DAT
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
         kns[j][15 - i] = tkns[j]; // zmiana kolejnosci kluczy z i na 15-i
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
/* Converts binary numbe to printed hexadecimal equivalent */
{
   int i, j, inte, weight;

   printf("DeCipher hex: ");
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
   for (int i = 0; i <= 15; i++)
   {
      ss << std::hex << int(inputText[i]);
   }
   std::cout << std::endl;
   outputText = ss.str();
}

std::string BINTOTEXT(gl64array jotput)
{
   std::string out;

   char outTab[9];

   for (int charIndex = 0; charIndex < 8; charIndex++)
   {
      outTab[charIndex] = 0;
      for (int bitOfCHarIndex = 0; bitOfCHarIndex < 8; bitOfCHarIndex++)
      {
         outTab[charIndex] <<= 1;
         outTab[charIndex] += jotput[(charIndex * 8) + bitOfCHarIndex];
      }
   }

   outTab[8] = 0;

   return std::string(outTab);
}

void writeDecipher(std::string out)
{
   std::ofstream file_out;
   file_out.open("decrypted.txt", std::ios_base::app);
   file_out << out;
   file_out.close();
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

   system("cls");
   glopen(&infile, "HEX.DAT");
   for (i = 0; i <= 15; i++)
      for (j = 0; j <= 3; j++)
         fscanf(infile, "%d", &(hexinbin[j][i]));

   std::string password;
   std::string hexPassword;
   std::cout << std::endl
             << "Podaj klucz deszyfrujacy: " << std::endl;
   printf("00000000\r");
   std::cin >> password;
   TEXTTOHEX(password, hexPassword);
   for (int i = 0; i < 16; i++)
   {
      hkey[i] = hexPassword.at(i);
   }
   hkey[16] = 0;
   HEXTOBIN(hkey, key, hexinbin);

   std::ifstream stream("encrypted.txt", std::ifstream::binary);
   if (stream)
   {
      ksflg = 1;
      cyflg = 1;
      desflg = 1;
      // get length of file:
      stream.seekg(0, stream.end);
      int length = stream.tellg();
      stream.seekg(0, stream.beg);

      char *buffer = new char[16];

      int iterations = length / 16;
      std::cout << "Liczba blokow:  " << iterations - 1 << std::endl;
      int full_blocks = iterations - 2;
      remove("decrypted.txt");
      for (full_blocks; full_blocks > 0; full_blocks--)
      {
         stream.read(buffer, 16);
         std::cout << "Block in hex:   " << buffer << std::endl;
         HEXTOBIN(buffer, inpt, hexinbin);

         d_des(inpt, key, jotput, desflg, ip, ipm, ksflg, ipc1, ipc2, glicd, cyflg, iet, ipp, is, ibin);
         std::string text_block = BINTOTEXT(jotput);
         std::cout << "DeCipher ASCII: " << text_block << std::endl;
         writeDecipher(text_block);
      }
      stream.read(buffer, 16);
      std::cout << "Block in hex:   " << buffer << std::endl;
      HEXTOBIN(buffer, inpt, hexinbin);

      d_des(inpt, key, jotput, desflg, ip, ipm, ksflg, ipc1, ipc2, glicd, cyflg, iet, ipp, is, ibin);
      std::string last_block = BINTOTEXT(jotput);
      stream.read(buffer, 16);
      HEXTOBIN(buffer, inpt, hexinbin);

      d_des(inpt, key, jotput, desflg, ip, ipm, ksflg, ipc1, ipc2, glicd, cyflg, iet, ipp, is, ibin);
      std::string control_block = BINTOTEXT(jotput);
      char c = control_block[0];
      int num = c - 40;

      if (num > 0) {
         std::cout << "DeCipher ASCII: ";
         std::string tmp;
         for(int i = 0; i < num; i++){
            std::cout << last_block[i];
            tmp += last_block[i];
         }
         std::cout << std::endl;
         writeDecipher(tmp);
      } else {
         std::cout << "DeCipher ASCII: " << last_block << std::endl;
         writeDecipher(last_block);
      }
      stream.close();

      delete[] buffer;
   }
}
