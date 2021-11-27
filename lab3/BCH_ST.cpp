// BCH_ST.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	n	15		/* dlugosc slowa kodowego		*/
#define	r	8		/* stopien wielomianu generujacego kod	*/
#define	d	5		/* minimalna odleglosc miedzy slowami	*/

/* wielomian generujucy kod     n = 31;   k = 21;   r = 10   d = 5 */

// int	wg[r + 1] = { 1,1, 1,0,1, 1,0,1, 0,0,1 };
           //      3     5      5      1
// wg[0] - wspolczynnik przy najwyzszej potedze


// inne kody cykliczne         n = 7;    k = 4;    r = 3;   d = 3

// int	wg[r+1] = {1,0, 1,1};     
     	        //   1    3    
// wg[0] - wspolczynnik przy najwyzszej potedze

// inne kody cykliczne    n = 15;    k = 7;    r = 8;   d = 5

 int	wg[r + 1] = { 1,1,1, 0,1,0, 0,0,1 };     
			  //        7      2      1
// wg[0] - wspolczynnik przy najwyzszej potedze




void Calculate_Syndrom(int slowo[], int syndrom[])
{
	/* wg[r+1]    - wielomian generujacy 	*/
	/* slowo[n]   - wektor kodowy        	*/
	/* syndrom[r] - obliczony syndrom    	*/

	//  dzielenie wielomianu zapisanego w slowo[] przez wg[]
	//  slowo[0],  wg[0]   - wspolczynniki przy najwyzszej potedze wielomianow
	// np. slowo[n] = {1,0,0,0,0,0,0}  wg[r+1] = { 1,0,1,1 }    syndrom[] = {1, 0, 1} = (slowo) mod (wg) reszta z dzielenia

	// wstawic obliczanie syndromu 

	int table[n];

	for (int i = 0; i < n; i++) {
		table[i] = slowo[i];
	}

	for (int j = 0; j < n - r; j++) {
		if (table[j] != 0) {
			for (int k = 0; k < r + 1; k++) {
				table[k + j] = table[k + j] ^ wg[k];
			}
		}
	}

	for (int i = 7; i < n; i++) {
    	syndrom[i - 7] = table[i];
  	}

	return;
}

int main()
{
	int	F2[n], C[n];	/* elementy ciagu 	*/
	int	F3[r];	/* elementy syndromu 	*/

	int	waga, h = 0, i, j, k;
	char	message[256] = "", error[256] = ""; 

	for (i = 0; i < 255; i++) { message[i] = 0;   error[i] = 0; }

	printf("------------------- Kod BCH -------------------\n");
	printf("                                                 ");
	for (i = 0; i < n - r; i++) printf("_");
	printf("\nPodaj ciag do wyslania (%3d znakow '0' lub '1'): ", n - r);
	
	std::cin >> message; 

		
	//scanf("%s", message);
	// fflush(stdin);

	i = strlen(message);

	if (i < n - r)
		for (j = i; j < n - r; j++) message[j] = '0';

	message[n - r] = '\0';

	for (i = 0; i < n - r; i++)
		F2[i] = message[i] - '0';

	for (i = n - r; i < n; i++)
		F2[i] = 0;					// dopisz zera na pozycjach kontrolnych

	Calculate_Syndrom(F2, F3);    // oblicz resztę F3(x) = F2(x) mod wg(x) 

	for (i = n - r; i < n; i++)        //  dopisz (normalnie dodajemy mod 2, ale byly zera na koncu wiec wystarczy dopisac) 
       F2[i] = F3[i - n + r];          //  resztę (syndrom) na koniec wektora - ostatnie r bitow to czesc kontrolna kodu
		       

	printf("\nZakodowany ciag (wektor kodowy  n = %d): ", n);
	for (i = 0; i < n; i++)
	{
		printf("%d", F2[i]); C[i] = F2[i];
	}

	printf("\n");

	printf("                                            ");
	for (i = 0; i < n; i++) printf("_");
	printf("\nPodaj blad wprowadzony do wektora kodowego: ");
	
	std::cin >> error; 

	//scanf_s("%s", error);
	// fflush(stdin);

	i = strlen(error);

	if (i < n)
		for (j = i; j < n; j++) error[j] = '0';

	error[n] = '\0';

	//getchar();

	for (i = 0; i < n; i++) {
		k = error[i] - '0';
		F2[i] = (F2[i] + k) % 2;
	}

	k = 0;						// zliczanie liczby bledow
	for (i = 0; i < n; i++) 
		if (error[i]=='1') k++;

	if (k > (d - 1) / 2)
	{
		printf("\nLiczba bledow %d przekracza zdolnosc korekcyjna kodu = %d", k, (d - 1) / 2);
		printf("\nMozliwe bledne dzialanie algorytmu uproszczonego dekodowania\n");
	}

		printf("\nOdebrany ciag: ");
	for (i = 0; i < n; i++)
		printf("%d", F2[i]);
	printf("\n");

	waga = 0;
	Calculate_Syndrom(F2, F3);
	for (i = 0; i < r; i++)
		if (F3[i] > 0) waga++;

	printf("\nSyndrom odebranego ciagu: ");
	for (i = 0; i < r; i++)
		printf("%d", F3[i]);
	printf("\n");

	printf("Waga syndromu: %d\n", waga);

	if (waga <= ((d - 1) / 2)) {
		printf("\nW ciagu informacyjnym nie ma bledu - nie ma potrzeby korekcji\n");
		printf("Nadany wektor kodowy:      ");
		for (i = 0; i < n; i++)
			printf("%d", C[i]);
		printf("\n");
		printf("Nadany ciag:      ");
		for (i = 0; i < n-r; i++)
			printf("%d", C[i]);
		printf("\n");
	}
	else {
		printf("\nW odebranym ciagu sa bledy - rozpoczynam korekcje\n\n");

		do {          // shift right
			h++;
			k = F2[n - 1];
			for (i = n - 2; i >= 0; i--)
				F2[i + 1] = F2[i];
			F2[0] = k;

			printf("Przesuniety ciag: ");
			for (i = 0; i < n; i++)
				printf("%d", F2[i]);
			printf("\n");

			waga = 0;
			Calculate_Syndrom(F2, F3);

			for (i = 0; i < r; i++)
				if (F3[i] > 0) waga++;

			printf("Syndrom ciagu odebranego: ");
			for (i = 0; i < r; i++)
				printf("%d", F3[i]);
			printf("\n");

			printf("Waga syndromu: %d\n", waga);

			printf("<Enter>\n");
			getchar();

		} while ((waga > ((d - 1) / 2)) && (h <= (n - r)));

		if (waga <= (d - 1) / 2) {
			for (i = n - r; i < n; i++)
				F2[i] = (F2[i] + F3[i - n + r]) % 2;
			do { //shift right n-h positions = shift left h positions
				h++;
				k = F2[n - 1];
				for (i = n - 2; i >= 0; i--)
					F2[i + 1] = F2[i];
				F2[0] = k;
			} while (h != n);

			printf("Koniec procesu korekcji.\nSkorygowany ciag: ");
			for (i = 0; i < n; i++)
				printf("%d", F2[i]);
			printf("\nNadany ciag:      ");
			for (i = 0; i < n; i++)
				printf("%d", C[i]);
			printf("\n");

		}
		else {
			printf("\nCiagu nie da sie skorygowac!\n");
		}
	}
	printf("\nKoniec programu!\n<Enter>\n");
	getchar();
}
