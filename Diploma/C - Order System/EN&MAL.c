#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)
#define LOVE_LETTERS_PIRCE 25.0
#define NUT_COOKIES_PRICE 20.0
#define ASSORTED_COOKIES_PRICE 15.0
#define DISCOUNT_MORE_THAN_1000 10/100.0
#define DISCOUNT_BETWEEN_500_TO_1000 5/100.0
#define SPECIAL_DISCOUNT 10/100.0

int language, noLoveLetters, noNutCookies, noAssortedCookies, totalLoveLetters = 0, totalNutCookies = 0, totalAssortedCookies = 0, day, year, month, noOrder = 1;
double loveLettersPrice, nutCookiesPrice, assortedCookiesPrice, sale = 0, total = 0, discount, specialDis = 0, totalSpecialDis = 0, totalDiscount = 0, deposit, totalDeposit = 0, netSale, balance;
char  decision, decision2;//decision one for start order or not;    decision2 for cancel order or not

//logo
void logo();

//Date
void date();
void dateBM();

//Menu
void menu();
void menuBM();

//ask user whether want to order or not
void order();
void orderBM();

//accept user order
void order2();
void order2BM();

//order note 
void orderNote();
void orderNoteBM();

//print the summary of that day
void summary();

//main system
void systemBM();
void systemEN();


void main() {
		//display title and logo
		logo();

		//ask user to select language
		do {
			rewind(stdin);
			printf("Please select language 1.Bahasa Melayu 2.English >");
			scanf("%d", &language);
			if (language == 1) {
				dateBM();
				break;
			}
			else if (language == 2) {
				date();
				break;
			}
			else {
				printf("Enter wrongly!!!!!!!\nPlease enter 1. Bahasa Melayu or 2.English\n\n");
			}
		} while (3 > 2);


		//Use the system in language that user prefer
		do {
			if (language == 1) {
				printf("\n");
				systemBM();
				
			}
			else  {
				printf("\n");
				systemEN();
				
			}
			
		} while (decision == 'y' || decision == 'Y');


	

	system("pause");
}

//logo
void logo() {
	printf("TARUC FESTIVE COOKIES SDN BHD        ________________________________________\n");
	printf("    COOKIES ORDERING SYSTEM          |  _______       _____  _    _  _____   |\n");
	printf("                                     | |__   __|/\\   |  __ \\| |  | |/ ____|  |\n");
	printf("                                     |    | |  /  \\  | |__) | |  | | |       |\n");
	printf("                                     |    | | / /\\ \\ |  _  /| |  | | |       |\n");
	printf("                                     |    | |/ ____ \\| | \\ \\| |__| | |____   |\n");
	printf("                                     |    |_/_/    \\_\\_|  \\_\\\\____/ \\_____|  |\n");
	printf("                                     |                                       |\n");
	printf("                                     |_______________________________________|\n\n");
}

//sytem in english
void systemEN() {
	menu();
	order();

}

//system in Bahasa Melayu
void systemBM() {
	menuBM();
	orderBM();
}


//Date
void date() {
	printf("\nPlease enter the current date(dd-mm-yy) > ");
	scanf("%d-%d-%d", &day, &month, &year);
	printf("\n============================================================================\n");
	printf("Today date:%d-%d-%d\n\n", day, month, year);
	
}

void dateBM() {
	printf("\nSila masukkan tarikh hari ini (hari-bulan-tahun) > ");
	scanf("%d-%d-%d", &day, &month, &year);
	printf("\n============================================================================\n");
	printf("Tarikh :%d-%d-%d\n\n", day, month, year);
	
}

//Menu
void menu() {

	printf("%20s\n", "Menu");
	printf("%20s\n", "====");
	printf("%-16s     %s     %s\n", "Category", "Unit", "Price(RM) per unit");
	printf("%-16s     %s     %s\n", "========", "====", "==================");
	printf("%-16s     %s      %13s%.2f\n", "Love Letters", "tin", "RM", LOVE_LETTERS_PIRCE);
	printf("%-16s     %s      %13s%.2f\n", "Nut Cookies", "jar", "RM", NUT_COOKIES_PRICE);
	printf("%-16s     %s      %13s%.2f\n\n", "Assorted Cookies", "jar", "RM", ASSORTED_COOKIES_PRICE);
	printf("*Purchase between RM500-RM1000 can get 5%% discount\n*Purchase above RM1000 can get 10%% discount\n");

	//Tell user that Hari Merdeka got special discount
	if (day == 31 && month == 8) {
		printf("\n*Today Hari Merdeka Special offer!!!,all order have discount 10%%!!!!!!\n\n");
	}
	else {
		printf("\n");
	}



}

void menuBM() {
	printf("%20s\n", "Menu");
	printf("%20s\n", "====");
	printf("%-16s     %s     %s\n", "Jenis", "Unit", "Harga seunit(RM) ");
	printf("%-16s     %s     %s\n", "========", "====", "=====================");
	printf("%-16s     %s      %13s%.2f\n", "Love Letters", "setin", "RM", LOVE_LETTERS_PIRCE);
	printf("%-16s     %s      %13s%.2f\n", "Nut Cookies", "sejar", "RM", NUT_COOKIES_PRICE);
	printf("%-16s     %s      %13s%.2f\n\n", "Assorted Cookies", "sejar", "RM", ASSORTED_COOKIES_PRICE);
	printf("*Belian antara RM500-RM1000 mempunyai diskaun 5%%\n*Setiap pesanan melebihi RM1000 mempunyai diskaun 10%%\n");

	//Tell user that Hari Merdeka got special discount
	if (day == 31 && month == 8) {
		printf("\n*Hari ini Hari Merdeka£¡£¡£¡£¡£¡Semua pesanan mempunyai diskaun 10%%£¡£¡£¡£¡£¡\n\n");
	}
	else {
		printf("\n");
	}
}


//ask user whether want to order or not
void order() {

		
		printf("============================================================================\n");
		do {
			//get user reply
			rewind(stdin);
			printf("Do you want to order? (Y/N) >");
			scanf("%c", &decision);


			if (decision == 'Y' || decision == 'y') {
				order2();
				break;
			}
			else if (decision == 'N' || decision == 'n') {
				summary();
				break;
			}
			else {
				printf("You enter wrongly,please enter again(Y/N) \n");
			}
		} while (3 > 2);
	}


void orderBM() {
	
	
	printf("============================================================================\n");
	do {
		//get user reply
		rewind(stdin);
		printf("Adakah anda hendak membuat pesanan ? (Y/N) >");
		scanf("%c", &decision);


		if (decision == 'Y' || decision == 'y') {
			order2BM();
			break;
		}
		else if (decision == 'N' || decision == 'n') {
			summary();
			break;
		}
		else {
			printf("Input yang dimasukan salah, sila masukan Y atau N \n");
		}

	} while (3 > 2);
}


//ask user to order what user want
void order2() {

	printf("\nOrder number:%d\n\n\n", noOrder);

	printf("%-32s Quantity\n", "Item");

	//get the user order for love letters
	printf("LOVE LETTERS @ RM %.2lf per tin :", LOVE_LETTERS_PIRCE);
	scanf("%d", &noLoveLetters);
	totalLoveLetters = totalLoveLetters + noLoveLetters;
	loveLettersPrice = noLoveLetters * LOVE_LETTERS_PIRCE;
	printf("\n%-32s:   RM%9.2lf\n\n", "Love Letter order", loveLettersPrice);

	//get the user order for nut cookies
	printf("NUT  COOKIES @ RM %.2lf per jar :", NUT_COOKIES_PRICE);
	scanf("%d", &noNutCookies);
	totalNutCookies = totalNutCookies + noNutCookies;
	nutCookiesPrice = noNutCookies * NUT_COOKIES_PRICE;
	printf("\n%-32s:   RM%9.2lf\n\n", "Nut Cookies order", nutCookiesPrice);

	//get the user order for assorted cookies
	printf("ASST COOKIES @ RM %.2lf per jar :", ASSORTED_COOKIES_PRICE);
	scanf("%d", &noAssortedCookies);
	totalAssortedCookies = totalAssortedCookies + noAssortedCookies;
	assortedCookiesPrice = noAssortedCookies * ASSORTED_COOKIES_PRICE;
	printf("\n%-32s:   RM%9.2lf\n", "Assorted Cookies order", assortedCookiesPrice);

	//calculate total
	total = assortedCookiesPrice + loveLettersPrice + nutCookiesPrice;

	//caluculate discount
	if (total > 1000) {
		discount = total * DISCOUNT_MORE_THAN_1000;
	}
	else if (total >= 500 && total < 1000) {
		discount = total * DISCOUNT_BETWEEN_500_TO_1000;
	}
	else {
		discount = 0.0;
	}

	//Hari Merdeka Special discount
	if (day == 31 && month == 8) {
		specialDis = total * SPECIAL_DISCOUNT;
		totalSpecialDis += specialDis;
	}

	totalDiscount = totalDiscount + discount;
	balance = total - discount - specialDis;
	sale = sale + total;
	printf("%32s    %9s\n", " ", "-----------");
	printf("%32s:   RM%9.2lf\n", "Order Total", total);
	printf("%32s:   RM%9.2lf\n", "Discount", discount);

	//print the Hari Merdeka special discount amount
	if (day == 31 && month == 8) {
		printf("%32s:   RM%9.2lf\n", "Hari Merdeka Special Discount", specialDis);
	}

	printf("%32s:   RM%9.2lf\n", "Amount to pay", balance);


	//repeat asking user to comfirm order if user enter wrongly
	do {
		rewind(stdin);
		printf("\n%32s:", "comfirm order?(Y/N)");
		scanf("%c", &decision2);

		if (decision2 == 'Y' || decision2 == 'y') {

			//repeat asking user to key in the deposit if user type the deposit wrongly
			do {
				printf("%32s:   RM", "Deposit");
				scanf("%lf", &deposit);


				if (deposit > balance) {
					printf("\nThe deposit is greater than balance you need to pay!\n");
					printf("Please enter the amount of deposit again!\n\n");

				}
				else {

					totalDeposit += deposit;

					printf("%32s:   RM%9.2lf\n", "Balance", balance - deposit);

					break;
				}
			} while (3 > 2);


		}
		else if (decision2 == 'N' || decision2 == 'n') {

			//cancel all the data store if user cancel order

			totalAssortedCookies -= noAssortedCookies;
			totalNutCookies -= noNutCookies;
			totalLoveLetters -= noLoveLetters;
			sale -= total;
			totalDiscount -= discount;
			totalSpecialDis -= specialDis;
			printf("\nYour order are cancelled\n\n");
			
			break;
		}
		else {
			printf("you enter wrongle please enter again!!\n");
		}
	} while (decision2 != 'Y' &&decision2 != 'y'&& decision2 != 'N'&&decision2 != 'n');

	if (decision2 == 'N' || decision2 == 'n') {
		
		do {
			printf("\n\n============================================================================\n\n");
			rewind(stdin);
			printf("Please select language 1. Bahasa Melayu 2.English >");
			scanf("%d", &language);
			if (language != 1 && language != 2) {
				printf("Enter wrongly please enter again\n");
			}
			
		} while (language != 1 && language != 2);
	}
	else
		orderNote();

}

void order2BM() {
	printf("\nnombor pesanan:%d\n\n\n", noOrder);

	printf("%-32s kuantiti\n", "Jenis");

	//get the user order for love letters
	printf("Love Letters setin RM %.2lf  :", LOVE_LETTERS_PIRCE);
	scanf("%d", &noLoveLetters);
	totalLoveLetters = totalLoveLetters + noLoveLetters;
	loveLettersPrice = noLoveLetters * LOVE_LETTERS_PIRCE;
	printf("\n%-32s:   RM%9.2lf\n\n", "Love Letters", loveLettersPrice);

	//get the user order for nut cookies
	printf("NUT  COOKIES sejar RM %.2lf  :", NUT_COOKIES_PRICE);
	scanf("%d", &noNutCookies);
	totalNutCookies = totalNutCookies + noNutCookies;
	nutCookiesPrice = noNutCookies * NUT_COOKIES_PRICE;
	printf("\n%-32s:   RM%9.2lf\n\n", "NUT  COOKIES", nutCookiesPrice);

	//get the user order for assorted cookies
	printf("ASST COOKIES sejar RM %.2lf  :", ASSORTED_COOKIES_PRICE);
	scanf("%d", &noAssortedCookies);
	totalAssortedCookies = totalAssortedCookies + noAssortedCookies;
	assortedCookiesPrice = noAssortedCookies * ASSORTED_COOKIES_PRICE;
	printf("\n%-32s:   RM%9.2lf\n", "ASST COOKIES", assortedCookiesPrice);

	//calculate total
	total = assortedCookiesPrice + loveLettersPrice + nutCookiesPrice;

	//caluculate discount
	if (total > 1000) {
		discount = total * DISCOUNT_MORE_THAN_1000;
	}
	else if (total >= 500 && total < 1000) {
		discount = total * DISCOUNT_BETWEEN_500_TO_1000;
	}
	else {
		discount = 0.0;
	}

	//Hari Merdeka Special discount
	if (day == 31 && month == 8) {
		specialDis = total * SPECIAL_DISCOUNT;
		totalSpecialDis += specialDis;
	}

	totalDiscount = totalDiscount + discount;
	balance = total - discount - specialDis;
	sale = sale + total;
	printf("%32s    %9s\n", " ", "-----------");
	printf("%32s:   RM%9.2lf\n", "Semua sekali", total);
	printf("%32s:   RM%9.2lf\n", "diskaun", discount);

	//print the Hari Merdeka special discount amount
	if (day == 31 && month == 8) {
		printf("%32s:   RM%9.2lf\n", "Diskaun Hari Merdeka", specialDis);
	}

	printf("%32s:   RM%9.2lf\n", "baki", balance);


	//repeat asking user to comfirm order if user enter wrongly
	do {
		rewind(stdin);
		printf("\n%32s:", "Sila memeriksa bahawa pesanan ini betulkah?(Y/N)");
		scanf("%c", &decision2);

		if (decision2 == 'Y' || decision2 == 'y') {

			//repeat asking user to key in the deposit if user type the deposit wrongly
			do {
				printf("%32s:   RM", "Deposit");
				scanf("%lf", &deposit);

				
				if (deposit > balance) {
					printf("\n%32s\n","Deposit melebihi baki!");
					printf("%32s\n\n","Sila masukkan semula!");

				}
				else {

					totalDeposit += deposit;

					printf("%32s:   RM%9.2lf\n", "masih tinggal", balance - deposit);

					break;
				}
			} while (3 > 2);


		}
		else if (decision2 == 'N' || decision2 == 'n') {

			///cancel all the data store if user cancel order
			totalAssortedCookies -= noAssortedCookies;
			totalNutCookies -= noNutCookies;
			totalLoveLetters -= noLoveLetters;
			sale -= total;
			totalDiscount -= discount;
			totalSpecialDis -= specialDis;
			printf("\npesanan anda telah dibatalkan\n\n");

			
			break;
		}
		else {
			printf("Input salah, sila masukkan semula!!\n");
		}
	} while (decision2 != 'Y' &&decision2 != 'y'&& decision2 != 'N'&&decision2 != 'n');

	
	if (decision2 == 'N' || decision2 == 'n') {
		do {
			printf("\n\n============================================================================\n\n");
			rewind(stdin);
			printf("Please select language 1. Bahasa Melayu 2.English >");
			scanf("%d", &language);
			if (language != 1 && language != 2) {
				printf("Enter wrongly please enter again\n");
			}
		} while (language != 1 && language != 2);
	}
	else
		orderNoteBM();
}


//print order note
void orderNote() {
	printf("\n\n============================================================================\n");
	printf("Cookies Ordering System of TARC Festive Cookies Sdn Bhd\n");
	printf("Order date  :%d-%d-%d\n", day, month, year);
	printf("Order number:%d\n\n\n", noOrder);
	printf("%4d LOVE LETTERS @RM%.2lf per tin : RM%9.2lf\n", noLoveLetters, LOVE_LETTERS_PIRCE, loveLettersPrice);
	printf("%4d NUT  COOKIES @RM%.2lf per jar : RM%9.2lf\n", noNutCookies, NUT_COOKIES_PRICE, nutCookiesPrice);
	printf("%4d ASST COOKIES @RM%.2lf per jar : RM%9.2lf\n", noAssortedCookies, ASSORTED_COOKIES_PRICE, assortedCookiesPrice);
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n", "ORDER TOTAL", total);
	printf("%35s: RM%9.2lf\n", "DISCOUNT", discount);
	if (day == 31 && month == 8) {
		printf("%35s: RM%9.2lf\n", "Hari Merdeka Special Discount", specialDis);
	}
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n", "AMOUNT TO PAY", balance);
	printf("%35s: RM%9.2lf\n", "DEPOSIT PAID", deposit);
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n\n", "BALANCE", balance - deposit);
	noOrder++;
	printf("Thank You for Your Order.\n");
	printf("Please come to take your cookie after 4 day.\n");
	printf("Don't forgot to bring this order note when collecting your cookies. :-)\n\n");
	printf("============================================================================\n");

	do {
		language = NULL;
		rewind(stdin);
		printf("Please select language 1.Bahasa Melayu 2.English >");
		scanf("%d", &language);

		if (language == 1) {

			break;
		}
		else if (language == 2) {

			break;
		}
		else {
			printf("Enter wrongly!!!!!!!\nPlease enter 1. Bahasa Melayu or 2.English\n\n");
		}

	} while (3 > 2);

}

void orderNoteBM() {
	printf("\n\n============================================================================\n");
	printf("TARC Festive Cookies Sdn Bhd Sistem pesan cookies\n");
	printf("Tarikh:%d-%d-%d\n", day, month, year);
	printf("Nombor pesanan:%d\n\n\n", noOrder);
	printf("%4d Love Letters setin RM %.2lf   : RM%9.2lf\n", noLoveLetters, LOVE_LETTERS_PIRCE, loveLettersPrice);
	printf("%4d NUT  COOKIES sejar RM %.2lf   : RM%9.2lf\n", noNutCookies, NUT_COOKIES_PRICE, nutCookiesPrice);
	printf("%4d ASST COOKIES sejar RM %.2lf   : RM%9.2lf\n", noAssortedCookies, ASSORTED_COOKIES_PRICE, assortedCookiesPrice);
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n", "Semua sekali", total);
	printf("%35s: RM%9.2lf\n", "Diskaun", discount);
	if (day == 31 && month == 8) {
		printf("%35s: RM%9.2lf\n", "Diskaun Hari Merdeka", specialDis);
	}
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n", "Baki", balance);
	printf("%35s: RM%9.2lf\n", "Deposit", deposit);
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n\n", "Masih tinggal", balance - deposit);
	noOrder++;
	printf("Terima kasih\n");
	printf("Sila datang ambil cookies selepas 4 hari\n");
	printf("Jangan lupa bawa nota debit ini :-)\n\n");
	printf("============================================================================\n");

	do {
		language = NULL;
		rewind(stdin);
		printf("Please select language 1.Bahasa Melayu 2.English >");
		scanf("%d", &language);

		if (language == 1) {

			break;
		}
		else if (language == 2) {

			break;
		}
		else {
			printf("Salah Input!!!!!!!\nSila input sekali lagi\n\n");
		}

	} while (3 > 2);
}


//print the total summary for that days
void summary() {


	printf("\n\n============================================================================\n\n");
	printf("                      Order Summary for today(%d-%d-%d)\n", day, month, year);
	printf("                      =======================================\n\n");
	printf("Total number of customer order:%d\n\n", noOrder - 1);

	printf("Total Order       Unit Price             Amount\n");
	printf("===========       ==========             =======\n");
	printf("%4d LOVE LETTERS @RM%.2lf per tin : RM%9.2lf\n", totalLoveLetters, LOVE_LETTERS_PIRCE, totalLoveLetters*LOVE_LETTERS_PIRCE);
	printf("%4d NUT  COOKIES @RM%.2lf per jar : RM%9.2lf\n", totalNutCookies, NUT_COOKIES_PRICE, totalNutCookies*NUT_COOKIES_PRICE);
	printf("%4d ASST COOKIES @RM%.2lf per jar : RM%9.2lf\n", totalAssortedCookies, ASSORTED_COOKIES_PRICE, totalAssortedCookies*ASSORTED_COOKIES_PRICE);
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n", "TOTAL SALE", sale);
	printf("%35s: RM%9.2lf\n", "TOTAL DISCOUNT", totalDiscount);
	if (day == 31 && month == 8) {
		printf("%35s: RM%9.2lf\n", "Total Hari Merdeka Special Discount", totalSpecialDis);
	}
	printf("%35s  ===========\n", " ");
	netSale = sale - totalDiscount - totalSpecialDis;

	printf("%35s: RM%9.2lf\n", "TOTAL NET SALE", netSale);
	printf("%35s: RM%9.2lf\n", "TOTAL DEPOSITS", totalDeposit);
	printf("%35s  ===========\n", " ");
	printf("%35s: RM%9.2lf\n\n", "TOTAL BALANCE", netSale - totalDeposit);
	
	
}
