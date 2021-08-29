#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>
#pragma warning(disable: 4996)

// global variable
char studName[50], studID[12], level[10], adminID[12];
int salaryGuar1, salaryGuar2, orderNum;
int adminRes, userRes, logout, chk15000 = 0;
bool chkReject = false;
int year, borrowAmount, interestRate, interestAmount, paybackAmount;
double monthlyPayback,leftAmount;

int splashScreen();
int fillLevelofStudy(char levelofStudy[]);
int fillGuardianDetail(char name[2][50], char ic[2][15], char phoneNum[2][15], int salary[]);
int checkWorkingStatus(char chkWork, char name[], char phoneNum[], char companyName[], char position[], int* salary);
int fillPersonalDetails(char phoneNum[], char email[], char* chkWork);

extern "C" {
	// C local functions:
	int changeFontRed();
	int setDefaultFont();
	int chkYear(char strYear[]);
	int getYear();
	int getValue(int a, int b, int c, int d, int e);
	int storeValue();
	int printBalanceStatment();
	int chkAction(char strAction[]);
	int getID(char inputID[]);
	int searchID(char inputPass[]);
	int checkApplication();
	int applicationRejectAction();
	int getAdminLoginResult(int result);
	int getUserLoginResult(int result);
	int getLogout(int result);
	int adminApproval();
	int clearScreen();
	int getLevelSalary();
	int getLevel();
	int getSalary1();
	int getSalary2();
	int fill_info();
	//yx
	int getAdminID(char inputID[]);
	void searchAdminID(char password[]);
	void getAmountLeft();
	int chkPaymentStatus();
	int checkPayBackStatus(char status[]);
	void changePayBackStatus();
	void resetPayBackStatus();
	int getAmount(int* totalPayBackAmount);
	void paymentInfoUpdate(int totalPayBackAmount, int paidAmount, int dueAmount);
	int getAllThreeAmount(int* totalPayBackAmount);
	int checkBankCardNumber(char cardNo[]);
	int checkCardCvv(int cvv);
	int enterAmount(int paymentAmount);
	double getDueAmount();
	int forgottenPassword(int userType);
	int passwordDecryption(char password[]);
	int passwordEncryption(char password[]);
	// external asm procedures:
	void LoginProcess();
	void AdminModule();
	void UserModule();
}

typedef struct {
	char id[11], pass[30], name[50];
}Student;

typedef struct {
	char id[11], pass[30];
}Admin;

struct Date
{
	int day, month, year;
};

typedef struct {
	char id[11];
	double paidAmount, totalPayBackAmount, dueAmount, monthlyAmount;
	struct Date date;
}PaymentInfo;

// function that used in cpp file
bool chkFileExist(char fileName[]) {
	FILE* fptr;
	if (fptr = fopen(fileName, "r")) {
		fclose(fptr);
		return 1;
	}
	return 0;
}

// function that used in asm file
int changeFontRed() {
	printf("\033[1;31m");
	return 0;
}

int setDefaultFont() {
	printf("\033[0m");
	return 0;
}

int getOrderNum() {
	char fileName[10] = "app", s1[7], file[15], txt[5] = ".txt", id[12], str[200];
	int num = 1, exist;

	FILE* fptr;

	do {
		exist = 0;
		sprintf(s1, "%d", num);
		strcpy(file, fileName);
		strcat(file, s1);
		strcat(file, txt);
		exist = chkFileExist(file);
		if (exist == 1) {
			fptr = fopen(file, "r");
			fscanf(fptr, "%[^ ] || %[^\n]\n", &id, &str);
			if (strcmp(id, studID) == 0) {
				orderNum = num;
				break;
			}
			fclose(fptr);
		}
		else {
			orderNum = 0;
		}
		num++;
	} while (exist == 1);

	return 0;
}

int checkFileRow(char fileName[]) {
	char str[100];
	int count = 0;
	FILE* fptr;

	fptr = fopen(fileName, "r");
	if (fptr == NULL) {
		printf("Unable to open the file.\n");
		exit(-1);
	}
	while (!feof(fptr)) {
		fscanf(fptr, "%[^\n]\n", &str);
		count++;
	}
	fclose(fptr);
	return count;
}

int chkYear(char strYear[]) {
	int i, count, rate;
	count = 0;

	for (i = 0; i < strlen(strYear); i++) {
		count++;
		if (isdigit(strYear[i]) == 0) {
			return 0;
			break;
		}
	}

	if (count == 1 || count == 2) {
		year = atoi(strYear);
		if (year > 0 && year < 16) {
			if (year > 0 && year <= 5) {
				rate = 1;
			}
			else if (year > 5 && year <= 10) {
				rate = 2;
			}
			else {
				rate = 3;
			}
			return rate;
		}
		else {
			return 0;
		}
	}
	return 0;
}

int getYear() {
	return year;
}

int getValue(int a, int b, int c, int d, int e) {
	borrowAmount = a;
	interestRate = b;
	interestAmount = c;
	paybackAmount = d;
	monthlyPayback = e/1000.0;
	return 0;
}

int storeValue() {
	char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt";
	FILE* fptr;

	getOrderNum();
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	fptr = fopen(file, "a");
	if (fptr == NULL) {
		printf("  Unable to open the file.\n");
		exit(-1);
	}
	fprintf(fptr, "%d || %d || %d || %d || %lf\n", borrowAmount, interestRate, interestAmount, paybackAmount, monthlyPayback);
	fclose(fptr);
}

int printBalanceStatment() {
	char fileName[10] = "app", s1[10], file[9], txt[5] = ".txt", str1[10];
	char email[60], phoneNo[15], empName[60], empPhone[15], comName[60], position[30];
	char guarName1[60], guarIC1[15], guarPhone1[15], guarName2[60], guarIC2[15], guarPhone2[15];
	int studSalary, i, retValue, status;
	
	FILE* fptr;

	status = chkPaymentStatus();
	getOrderNum();
	getAmountLeft();
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	retValue = checkApplication();
	if (retValue == 3) {
		fptr = fopen(file, "r");
		if (fptr == NULL) {
			printf("  Unable to open the file.\n");
			exit(-1);
		}
		fscanf(fptr, "%[^ ] || %[^||] || %[^ ] || %[^ ] || %[^\n]\n", &studID, &studName, &email, &phoneNo, &level);
		if (status == 1) {
			if (checkFileRow(file) == 6) {
				fscanf(fptr, "%[^||] || %[^ ] || %[^||] || %[^||] || %d\n", &empName, &empPhone, &comName, &position, &studSalary);
			}
			fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n", &guarName1, &guarIC1, &guarPhone1, &salaryGuar1);
			fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n%s\n", &guarName2, &guarIC2, &guarPhone2, &salaryGuar2, &str1);
			fscanf(fptr, "%d || %d || %d || %d || %lf\n", &borrowAmount, &interestRate, &interestAmount, &paybackAmount, &monthlyPayback);
			fclose(fptr);
		}
		else  {
			if (checkFileRow(file) == 7) {
				fscanf(fptr, "%[^||] || %[^ ] || %[^||] || %[^||] || %d\n", &empName, &empPhone, &comName, &position, &studSalary);
			}
			fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n", &guarName1, &guarIC1, &guarPhone1, &salaryGuar1);
			fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n%s\n", &guarName2, &guarIC2, &guarPhone2, &salaryGuar2, &str1);
			fscanf(fptr, "%d || %d || %d || %d || %lf\n", &borrowAmount, &interestRate, &interestAmount, &paybackAmount, &monthlyPayback);
			fclose(fptr);
		}

		printf("\n\n\n\t\t\t<< Balance Statement >>\n");
		printf(" ====================================================================\n\n");
		printf("  Student ID               : %s\n", studID);
		printf("  Student Name             : %s\n", studName);
		printf("  Student Email            : %s\n", email);
		printf("  Student Phone Number     : %s\n", phoneNo);
		printf("  Student Level of study   : %s\n\n", level);

		if (status == 1) {
			if (checkFileRow(file) == 6) {
				printf("  Employer Name            : %s\n", empName);
				printf("  Employer Phone Number    : %s\n", empPhone);
				printf("  Company Name             : %s\n", comName);
				printf("  Position                 : %s\n", position);
				printf("  Salary                   : RM %d\n\n", studSalary);
			}
		}
		else {
			if (checkFileRow(file) == 7) {
				printf("  Employer Name            : %s\n", empName);
				printf("  Employer Phone Number    : %s\n", empPhone);
				printf("  Company Name             : %s\n", comName);
				printf("  Position                 : %s\n", position);
				printf("  Salary                   : RM %d\n\n", studSalary);
			}
		}

		printf("  Guardian 1 Name          : %s\n", guarName1);
		printf("  Guardian 1 IC Number     : %s\n", guarIC1);
		printf("  Guardian 1 Phone Number  : %s\n", guarPhone1);
		printf("  Guardian 1 Salary        : RM %d\n\n", salaryGuar1);
		printf("  Guardian 2 Name          : %s\n", guarName2);
		printf("  Guardian 2 IC Number     : %s\n", guarIC2);
		printf("  Guardian 2 Phone Number  : %s\n", guarPhone2);
		printf("  Guardian 2 Salary        : RM %d\n\n", salaryGuar2);

		printf("  -------------------------------------------------------------------\n\n");
		printf("  Amount Borrow            : RM %d.00\n", borrowAmount);
		printf("  Interest Rate            : %d%%\n", interestRate);
		printf("  Interest Amount          : RM %d.00\n", interestAmount);
		printf("  Payback Amount           : RM %d.00\n", paybackAmount);
		printf("  Payback per Month        : RM %.2lf\n", monthlyPayback);
		printf("  Amount Left to Pay	   : RM %.2lf\n\n",leftAmount);

		system("pause");
		system("cls");
		return 0;
	}
	else {
		return 1;
	}
	return 0;
}

int chkAction(char strAction[]) {
	int i, count, action;
	count = 0;

	for (i = 0; i < strlen(strAction); i++) {
		count++;
		if (isdigit(strAction[i]) == 0) {
			return 0;
			break;
		}
	}

	if (count == 1) {
		action = atoi(strAction);
		if (action > 0 && action < 5) {
			return action;
		}
		else {
			return 0;
		}
	}
	return 0;
}

int getID(char inputID[]) {
	strcpy(studID, inputID);
	return 0;
}

int searchID(char inputPass[]) {
	Student stud[100];
	int i = 0, numData, foundID = 0;
	char tempPass[20];
	FILE* fptr;

	fptr = fopen("id_pass_name.txt", "r");
	if (fptr == NULL)
	{
		printf("Unable to open the file.\n");
		exit(-1);
	}

	fscanf(fptr, "%[^,],%[^,],%[^\n]\n", &stud[i].id, &stud[i].pass, &stud[i].name);
	while (!feof(fptr)) {
		i++;
		fscanf(fptr, "%[^,],%[^,],%[^\n]\n", &stud[i].id, &stud[i].pass, &stud[i].name);
	}
	fclose(fptr);

	numData = i;

	for (i = 0; i <= numData; i++) {
		if (strcmp(stud[i].id, studID) == 0) {
			strcpy(tempPass, stud[i].pass);
			passwordDecryption(tempPass);
			if (strcmp(tempPass, inputPass) == 0) {
				strcpy(studName, stud[i].name);
				getOrderNum();
				return 0;
			}
			else {
				return 2;
			}
			foundID = 1;
		}
	}

	if (foundID == 0) {
		return 1;
	}

	return 0;
}

int checkApplication() {
	char fileName[10] = "app", s1[10], file[10], txt[5] = ".txt", str[100];
	bool found = false;
	
	FILE* fptr;

	if (chk15000 == 1) {
		getOrderNum();
	}
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	fptr = fopen(file, "r");
	if (fptr == NULL) {
		return 0;
		exit(-1);
	}
	while (!feof(fptr)) {
		fscanf(fptr, "%s\n", &str);
		if (strcmp(str, "Approved") == 0 || strcmp(str, "Rejected") == 0) {
			found = true;
			if (strcmp(str, "Rejected") == 0) {
				return 4;
			}
			else {
				if (fscanf(fptr, "%s\n", &str) != EOF) {
					return 3;
				}
			}
			return 2;
		}
	}
	fclose(fptr);

	if (found == false) {
		return 1;
	}

	return 0;
}

int applicationRejectAction() {
	char yn, chkChar;

	printf("  Do you want to fill in application information again (y/n) ? ");
	rewind(stdin);
	scanf("%c", &yn);
	chkChar = getchar();
	if (chkChar != '\n' && chkChar != EOF) {
		printf("  Invalid input, please enter again.\n\n");
		return 1;
	}
	else {
		if (toupper(yn) != 'Y' && toupper(yn) != 'N') {
			printf("  Invalid input, please enter again.\n\n");
			return 1;
		}
		else if (toupper(yn) == 'Y') {
			chkReject = true;
			return 2;
		}
	}
	return 0;
}

int getAdminLoginResult(int result) {
	adminRes = result;
	return 0;
}

int getUserLoginResult(int result) {
	userRes = result;
	return 0;
}

int getLogout(int result) {
	logout = result;
	return 0;
}

int adminApproval() {
	FILE* fptr, * fptr1, * fptr2;
	char app[] = "Approved", rej[] = "Rejected";
	char str[150], fileName[10] = "app", s1[10], file[9], txt[5] = ".txt";
	int num = 1, i = 0, display = 0;
	bool exist, found = true;

	char getStatus, chkChar;

	do {
		sprintf(s1, "%d", num);
		strcpy(file, fileName);
		strcat(file, s1);
		strcat(file, txt);
		exist = chkFileExist(file);
		if (exist == true) {
			num++;
			found = false;
			fptr = fopen(file, "r");
			if (fptr == NULL) {
				printf("Unable to open the file.\n");
				exit(-1);
			}

			while (!feof(fptr)) {
				fscanf(fptr, "%s\n", &str);
				if (strcmp(str, app) == 0 || strcmp(str, rej) == 0) {
					found = true;
				}
			}
			fclose(fptr);

			if (found == false) {
				display = 1;
				i++;
				if (i == 1) {
					printf("\n  ==============================================================================");
				}
				printf("\n  Application %d:\n\n", i);
				fptr1 = fopen(file, "r");
				while (!feof(fptr1)) {
					fscanf(fptr1, "%[^\n]\n", &str);
					printf("  %s\n", str);
				}

				do {
					printf("\n  Approve(A) / Reject(R) ? ");
					rewind(stdin);
					scanf("%c", &getStatus);
					chkChar = getchar();
					if (chkChar != '\n' && chkChar != EOF) {
						printf("  Invalid input, please enter again.\n");
					}
					else {
						fptr2 = fopen(file, "a");
						if (toupper(getStatus) == 'A') {
							fprintf(fptr2, "%s\n", app);
							printf("\n  Successfully update the status.\n");
							printf("\n  --------------------------------------------------------------------\n");
						}
						else if (toupper(getStatus) == 'R') {
							fprintf(fptr2, "%s\n", rej);
							printf("\n  Successfully update the status.\n");
							printf("\n  --------------------------------------------------------------------\n");
						}
						else {
							printf("  Invalid input, please enter again.\n");
						}
						fclose(fptr2);
					}
				} while ((chkChar != '\n' && chkChar != EOF) || (toupper(getStatus) != 'A' && toupper(getStatus) != 'R'));
				fclose(fptr1);
			}
		}
	} while (exist == true);

	if (found == true || display == 0) {
		return 1;
	}
	return 0;
}

int clearScreen() {
	printf("\n");
	system("pause");
	system("cls");
	return 0;
}

int getLevelSalary() {
	char fileName[10] = "app", s1[10], file[9], txt[5] = ".txt";
	char id[11], name[60], email[50], phone[15], str1[100], gName1[60], gIC1[15], gPhone1[15], gName2[60], gIC2[15], gPhone2[15];
	FILE* fptr;

	getOrderNum();
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	int rowNum = checkFileRow(file);
	fptr = fopen(file, "r");
	if (fptr == NULL) {
		printf("Unable to open the file.\n");
		exit(-1);
	}
	fscanf(fptr, "%[^ ] || %[^||] || %[^ ] || %[^ ] || %[^\n]\n", &id, &name, &email, &phone, &level);
	if (rowNum == 5) {
		fscanf(fptr, "%[^\n]\n", &str1);
	}
	fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n", &gName1, &gIC1, &gPhone1, &salaryGuar1);
	fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n", &gName2, &gIC2, &gPhone2, &salaryGuar2);
	fclose(fptr);

	return 0;
}

int getLevel() {
	if (strcmp(level, "Diploma") == 0) {
		return 1;
	}
	return 0;
}

int getSalary1() {
	return salaryGuar1;
}

int getSalary2() {
	return salaryGuar2;
}

int fill_info() {
	int applicantSalary;
	char applicantPhone[15], email[80], chkWork, employerName[50], employerPhone[15], companyName[50], position[50], level[10];
	int guardianSalary[2];
	char guardianName[2][50], ic[2][15], guardianPhone[2][15];

	int x;
	char confirm, chkChar;
	char fileName[10] = "app", s1[10], file[9], txt[5] = ".txt";
	int num = 1;
	bool exist;

	FILE* fptr;

	do {
		system("cls");
		fillPersonalDetails(applicantPhone, email, &chkWork);
		checkWorkingStatus(chkWork, employerName, employerPhone, companyName, position, &applicantSalary);
		fillGuardianDetail(guardianName, ic, guardianPhone, guardianSalary);
		fillLevelofStudy(level);

		//Print out all the useful information that entered
		printf("\n\n\t\t\t  << Information Checking Process >>\n");
		printf(" ================================================================================\n\n");

		printf("\tPersonal Information\n");
		printf("\t--------------------\n");
		printf("  ID number     : %s\n", studID);
		printf("  Name          : %s\n", studName);
		printf("  Email Address : %s\n", email);
		printf("  Phone Number  : %s\n", applicantPhone);
		printf("  Level of Study: %s\n\n", level);

		if (toupper(chkWork) == 'Y') {
			printf("");

			printf("\n\t Working Information\n");
			printf("\t -------------------\n");

			printf("  Currently, the applicant is still working (full-time)\n");
			printf("  Name of employer        : %s\n", employerName);
			printf("  Phone number of employer: %s\n", employerPhone);
			printf("  Company name            : %s\n", companyName);
			printf("  Current position        : %s\n", position);
			printf("  Salary per month        : RM %d\n\n", applicantSalary);
		}

		printf("\n\tGuardian Information\n");
		printf("\t--------------------\n");

		for (x = 0; x < 2; x++) {
			printf("  Guardian %d:\n", x + 1);
			printf("  Name            : %s\n", guardianName[x]);
			printf("  IC Number       : %s\n", ic[x]);
			printf("  Phone number    : %s\n", guardianPhone[x]);
			printf("  Salary per month: RM %d\n\n", guardianSalary[x]);
		}

		do {
			printf("  Do you sure to submit this information (y/n)? ");
			rewind(stdin);
			scanf("%c", &confirm);
			chkChar = getchar();
			if (chkChar != '\n' && chkChar != EOF) {
				printf("  Invalid input, please enter again.\n");
			}
			else {
				if (toupper(confirm) != 'Y' && toupper(confirm) != 'N') {
					printf("  Invalid input, please enter again.\n");
				}
				else if (toupper(confirm) == 'Y') {
					do {
						exist = false;
						if (chkReject == true) {
							chkReject = false;
							num = orderNum;
							sprintf(s1, "%d", num);
							strcpy(file, fileName);
							strcat(file, s1);
							strcat(file, txt);
							fptr = fopen(file, "w");
							if (fptr == NULL) {
								printf("Unable to open the file.\n");
								exit(-1);
							}
							fprintf(fptr, "%s || %s || %s || %s || %s\n", studID, studName, email, applicantPhone, level);
							if (toupper(chkWork) == 'Y') {
								fprintf(fptr, "%s || %s || %s || %s || %d\n", employerName, employerPhone, companyName, position, applicantSalary);
							}
							for (x = 0; x < 2; x++) {
								fprintf(fptr, "%s || %s || %s || %d\n", guardianName[x], ic[x], guardianPhone[x], guardianSalary[x]);
							}
							fclose(fptr);
							break;
						}
						else {
							sprintf(s1, "%d", num);
							strcpy(file, fileName);
							strcat(file, s1);
							strcat(file, txt);

							exist = chkFileExist(file);
							if (exist == true) {
								num++;
							}
							else {
								exist = false;
								fptr = fopen(file, "w");
								if (fptr == NULL) {
									printf("Unable to open the file.\n");
									exit(-1);
								}
								fprintf(fptr, "%s || %s || %s || %s || %s\n", studID, studName, email, applicantPhone, level);
								if (toupper(chkWork) == 'Y') {
									fprintf(fptr, "%s || %s || %s || %s || %d\n", employerName, employerPhone, companyName, position, applicantSalary);
								}
								for (x = 0; x < 2; x++) {
									fprintf(fptr, "%s || %s || %s || %d\n", guardianName[x], ic[x], guardianPhone[x], guardianSalary[x]);
								}
								fclose(fptr);
								break;
							}
						}
					} while (exist == true);
					printf("\n  Successfully sent to admin side for verification.\n\n");
					system("pause");
					system("cls");
				}
				else {
					return 1;
				}
			}
		} while ((chkChar != '\n' && chkChar != EOF) || (toupper(confirm) != 'Y' && toupper(confirm) != 'N'));
	} while (toupper(confirm) == 'N');

	return 0;
}

int checkSalary15000() {
	char fileName[10] = "app", s1[10], file[9], txt[5] = ".txt", str1[200], str2[200];
	char gName1[60], gName2[60], gIC1[15], gIC2[15], gPhone1[15], gPhone2[15];
	int num = 1, sum, retValue;
	bool exist = false, found = false;

	FILE* fptr, * fptr1;
	do {
		sprintf(s1, "%d", num);
		strcpy(file, fileName);
		strcat(file, s1);
		strcat(file, txt);
		exist = chkFileExist(file);
		if (exist == true) {
			orderNum = num;
			retValue = checkApplication();
			if (retValue == 1) {
				int rowNum = checkFileRow(file);
				fptr = fopen(file, "r");
				if (fptr == NULL) {
					printf("Unable to open the file.\n");
					exit(-1);
				}
				fscanf(fptr, "%[^\n]\n", &str1);
				if (rowNum == 4) {
					fscanf(fptr, "%[^\n]\n", &str2);
				}
				fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n", &gName1, &gIC1, &gPhone1, &salaryGuar1);
				fscanf(fptr, "%[^||] || %[^ ] || %[^ ] || %d\n", &gName2, &gIC2, &gPhone2, &salaryGuar2);
				sum = salaryGuar1 + salaryGuar2;
				if (sum > 15000) {
					fptr1 = fopen(file, "a");
					if (fptr1 == NULL) {
						printf("Unable to open the file.\n");
						exit(-1);
					}
					fprintf(fptr1, "Rejected\n");
					fclose(fptr1);
				}
				fclose(fptr);
			}
			num++;
		}
	} while (exist == true);
	chk15000 = 1;

	return 0;
}

int main() {
	splashScreen();
	do {
		adminRes = 0;
		userRes = 0;
		logout = 0;

		checkSalary15000();

		LoginProcess();
		if (adminRes == 1) {
			AdminModule();
		}
		if (userRes == 1) {
			UserModule();
		}
		system("cls");
	} while (logout == 1);

	return 0;
}

int chkPaymentStatus() {

	int payBackStatus;
	char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt", unknown[200], status[20];

	FILE* fp1;

	getOrderNum();
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	int exist = chkFileExist(file);
	if (exist == 1) {
		fp1 = fopen(file, "r");

		if (fp1 == NULL)
		{
			printf("  Unable to open the file.\n");
			exit(-1);
		}

		while (fscanf(fp1, "%[^\n]\n", &unknown) != EOF)
		{
			if (!strcmp(unknown, "InProgress")) {
				strcpy(status, "InProgress");
			}
			else if (!strcmp(unknown, "PaidAll")) {
				strcpy(status, "PaidAll");
			}
			else
				strcpy(status, "FirstTime");
		}
		fclose(fp1);
		payBackStatus = checkPayBackStatus(status);
		return payBackStatus;
	}
}

int checkPayBackStatus(char status[]) {
	if (!strcmp("FirstTime", status))
		return 1;
	else if (!strcmp("InProgress", status))
		return 2;
	else if (!strcmp("PaidAll", status))
		return 0;
}

void resetPayBackStatus() {
	char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt", status[20] = "FirstTime";
	char str0[200], str1[200], str2[200], str3[200], str4[200], str5[200], str6[200];
	int isTrue = 0;
	FILE* fp1, * fp2;
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	fp1 = fopen(file, "r");
	if (fp1 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}

	while (!feof(fp1)) {
		if (checkFileRow(file) == 7) {
			fscanf(fp1, "%[^\n]\n", str0);
			isTrue = 1;
		}
		fscanf(fp1, "%[^\n]\n", str1);
		fscanf(fp1, "%[^\n]\n", str2);
		fscanf(fp1, "%[^\n]\n", str3);
		fscanf(fp1, "%[^\n]\n", str4);
		fscanf(fp1, "%[^\n]\n", str5);
		fscanf(fp1, "%[^\n]\n", str6);
	}
	fclose(fp1);

	fp1 = fopen(file, "w");
	if (fp1 == NULL)
	{
		printf("Unable to open a temporary file to write!!\n");
		fclose(fp1);
	}
	if (isTrue == 1)
		fprintf(fp1, "%s\n%s\n%s\n%s\n%s\n%s\n", str0, str1, str2, str3, str4, str5);
	else
		fprintf(fp1, "%s\n%s\n%s\n%s\n%s\n", str1, str2, str3, str4, str5);

	fclose(fp1);


	fclose(fp1);
	printf("  Pay back status in %s is updated back to <FirstTime>\n", file);
	printf("\n");


}

void changePayBackStatus() {
	char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt", unknown[200], status[20] = "PaidAll";
	char str0[200], str1[200], str2[200], str3[200], str4[200], str5[200], str6[200];
	int isTrue = 0;
	FILE* fp1;
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);

	fp1 = fopen(file, "r");
	if (fp1 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}

	while (!feof(fp1)) {
		if (checkFileRow(file) == 7) {
			fscanf(fp1, "%[^\n]\n", str0);
			isTrue = 1;
		}
		fscanf(fp1, "%[^\n]\n", str1);
		fscanf(fp1, "%[^\n]\n", str2);
		fscanf(fp1, "%[^\n]\n", str3);
		fscanf(fp1, "%[^\n]\n", str4);
		fscanf(fp1, "%[^\n]\n", str5);
		fscanf(fp1, "%[^\n]\n", str6);
	}
	fclose(fp1);

	fp1 = fopen(file, "w");
	if (fp1 == NULL)
	{
		printf("Unable to open a file to write!!\n");
		fclose(fp1);
	}
	if (isTrue == 1)
		fprintf(fp1, "%s\n%s\n%s\n%s\n%s\n%s\n", str0, str1, str2, str3, str4, str5);
	else
		fprintf(fp1, "%s\n%s\n%s\n%s\n%s\n", str1, str2, str3, str4, str5);

	fclose(fp1);

	fp1 = fopen(file, "a");
	if (fp1 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}

	fprintf(fp1, "%s\n", &status);
	fclose(fp1);
	printf("  Pay back status in %s is updated to <PaidAll>\n", file);
	printf("\n");

}

int checkBankCardNumber(char cardNo[]) {
	if (cardNo[0] == '4' && (strlen(cardNo) == 13 || strlen(cardNo) == 16)) {
		return 0;
	}
	else if (cardNo[5] == '5' && strlen(cardNo) == 13)
		return 0;
	else return 1;
}

int checkCardCvv(int cvv) {
	if (cvv >= 100 && cvv <= 999)
		return 0;
	else return 1;
}

int getAmount(int* totalPayBackAmount) {

	int i = 0, count = 6, counter = 0, monthlyAmountInt = 0, totalAmountInt = 0, rate = 0, rateAmount = 0, totalAmountwithRate = 0;
	char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt", status[20] = "InProgress";
	char newFileName[10], str[200], str1[200], str2[200], str3[200], str4[200];
	double monthlyAmountDouble = 0;
	FILE* fp1;
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);

	fp1 = fopen(file, "r");
	if (fp1 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}
	while (!feof(fp1)) {
		fscanf(fp1, "%[^\n]\n", str);
		if (checkFileRow(file) == 6) {
			fscanf(fp1, "%[^\n]\n", str1);
		}
		fscanf(fp1, "%[^\n]\n%[^\n]\n%[^\n]\n%d || %d || %d || %d || %lf\n", str2, str3, str4, &totalAmountInt, &rate, &rateAmount, &totalAmountwithRate, &monthlyAmountDouble);
	}
	fclose(fp1);
	if (*totalPayBackAmount != -1) {
		fp1 = fopen(file, "a");
		if (fp1 == NULL)
		{
			printf("File error.\n");
			exit(-1);
		}

		fprintf(fp1, "%s\n", &status);

		fclose(fp1);
		printf("  Pay back status in %s is updated to <InProgress>\n", file);
	}
	printf("\n");
	monthlyAmountInt = monthlyAmountDouble * 1000;
	counter = getValue(totalAmountInt, rate, rateAmount, totalAmountwithRate, monthlyAmountInt);
	*totalPayBackAmount = totalAmountwithRate * 100;
	monthlyAmountInt = monthlyAmountDouble * 100;
	return monthlyAmountInt;
}

void paymentInfoUpdate(int totalPayBackAmount, int paidAmount, int dueAmount) {
	SYSTEMTIME t;
	int year, month, day;
	int i = 0, count = 0, counter = 0, monthlyAmountInt = 0, totalAmountInt = 0, rate = 0, rateAmount = 0, totalAmountwithRate = 0;
	char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt", status[20] = "InProgress";
	char newFileName[10], str[200], str1[200], str2[200], str3[200], str4[200];
	char paymentFile[20];
	double monthlyAmountDouble = 0, totalPayBackAmt, paidAmt, dueAmt = 0;

	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);
	strcpy(paymentFile, studID);
	strcat(paymentFile, txt);

	GetLocalTime(&t);
	year = t.wYear;
	month = t.wMonth;
	day = t.wDay;

	totalPayBackAmt = (double)totalPayBackAmount / 100;
	paidAmt = (double)paidAmount / 100;
	dueAmt = (double)dueAmount / 100;
	FILE* fp1;
	fp1 = fopen(file, "r");
	if (fp1 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}

	while (!feof(fp1)) {
		fscanf(fp1, "%[^\n]\n", str);
		if (checkFileRow(file) == 7) {
			fscanf(fp1, "%[^\n]\n", str1);
		}
		fscanf(fp1, "%[^\n]\n%[^\n]\n%[^\n]\n%d || %d || %d || %d || %lf\n", str2, str3, str4, &totalAmountInt, &rate, &rateAmount, &totalAmountwithRate, &monthlyAmountDouble);
		count++;
	}
	fclose(fp1);
	//if (monthlyAmountDouble < mthlyPayBackAmt) {
	//	dueAmount = mthlyPayBackAmt - monthlyAmountInt;
	//}

	fp1 = fopen(paymentFile, "a");

	if (fp1 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}

	fprintf(fp1, "%s,%.2f,%.2f,%.2f,%.2f,%d/%d/%d\n", studID, paidAmt, monthlyAmountDouble, totalPayBackAmt, dueAmt, year, month, day);
	fclose(fp1);
	printf("\n");

}

int getAllThreeAmount(int* totalPayBackAmount) {
	int i = 0, count = 0, monthlyAmountInt = 0, dueMonth;
	int day, year, month, monthDiff;
	double dueAmount = 0, dueMonthAmount = 0, totalDueAmt = 0;
	char txt[5] = ".txt", paymentFile[20];
	strcpy(paymentFile, studID);
	strcat(paymentFile, txt);
	PaymentInfo info[100];

	SYSTEMTIME t;
	GetLocalTime(&t);
	year = t.wYear;
	month = t.wMonth;
	day = t.wDay;

	FILE* fp2;
	fp2 = fopen(paymentFile, "r");
	if (fp2 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}
	rewind(stdin);
	while (fscanf(fp2, "%[^,],%lf,%lf,%lf,%lf,%d/%d/%d\n", &info[i].id, &info[i].paidAmount, &info[i].monthlyAmount, &info[i].totalPayBackAmount, &info[i].dueAmount, &info[i].date.year, &info[i].date.month, &info[i].date.day) != EOF)
	{
		i++;
	}
	fclose(fp2);
	count = i - 1;
	dueAmount = info[count].dueAmount;

	if (info[count].date.year == year) {
		dueMonth = month - info[count].date.month;
		if (dueMonth > 0) {
			--dueMonth;
			dueMonthAmount = (dueMonth * info[count].monthlyAmount);
			totalDueAmt = dueAmount + dueMonthAmount;
			monthlyAmountInt = (totalDueAmt + info[count].monthlyAmount) * 100;
			if (*totalPayBackAmount != -1) {
				printf("  Due amount from last record is RM%.2f\n", dueAmount);
				printf("  Due amount after adding %d due month is RM%.2f\n", dueMonth, totalDueAmt);
				printf("  Monthly payment is RM%.2f\n", info[count].monthlyAmount);
				printf("\n  Total payment for this month : RM%.2f\n", info[count].monthlyAmount + totalDueAmt);
			}


		}
		else if (dueMonth == 0) {
			dueAmount = getDueAmount();
			if (dueAmount == 0) {
				monthlyAmountInt = 0;
			}
			else {
				if (*totalPayBackAmount != -1)
					printf("\n  Due amount for this month: %.2f\n", dueAmount);
				monthlyAmountInt = dueAmount * 100;
			}
		}

	}
	else if (info[count].date.year < year) {
		monthDiff = (year - info[count].date.year) * 12;
		dueMonth = month - info[count].date.month + monthDiff;
		if (dueMonth > 0) {
			--dueMonth;
			dueMonthAmount = (dueMonth * info[count].monthlyAmount);
			totalDueAmt = dueAmount + dueMonthAmount;
			monthlyAmountInt = (totalDueAmt + info[count].monthlyAmount) * 100;
			if (*totalPayBackAmount != -1) {
				printf("  Due amount from last record is RM%.2f\n", dueAmount);
				printf("  Due amount after adding %d due month is RM%.2f\n", dueMonth, totalDueAmt);
				printf("  Monthly payment is RM%.2f\n", info[count].monthlyAmount);
				printf("\n  Total payment for this month : RM%.2f\n", info[count].monthlyAmount + totalDueAmt);
			}

		}
		else if (dueMonth == 0) {
			dueAmount = getDueAmount();
			if (dueAmount == 0) {
				monthlyAmountInt = 0;
			}
			else {
				if (*totalPayBackAmount != -1)
					printf("  Due amount for this month: %.2f\n", dueAmount);
				monthlyAmountInt = dueAmount * 100;
			}

		}

	}
	*totalPayBackAmount = info[count].totalPayBackAmount * 100;
	return monthlyAmountInt;
}

int enterAmount(int paymentStatus) {
	double amount, amountInt, maxAmount, monthlyAmountDouble = 0;
	int totalAmountwithRate = 0, maxAmountInt = 0;

	switch (paymentStatus) {
	case 1: {
		maxAmount = monthlyPayback;
		break;
	}
	case 2:
		totalAmountwithRate = -1;
		maxAmountInt = getAllThreeAmount(&totalAmountwithRate);
		maxAmount = (double)maxAmountInt / 100;
		break;
	}


	printf("  Enter amount you want to pay >>> ");
	scanf("%lf", &amount);
	if (amount > maxAmount) {
		printf("  *Please enter amount not more than RM%.2f*\n\n", maxAmount);
		return 0;
	}
	else {
		amountInt = amount * 100;
		return amountInt;
	}
}

double getDueAmount() {
	PaymentInfo info[100];
	int i = 0, count = 0;
	double dueAmount = 0;
	char txt[5] = ".txt", paymentFile[20];
	strcpy(paymentFile, studID);
	strcat(paymentFile, txt);


	FILE* fp2;
	fp2 = fopen(paymentFile, "r");
	if (fp2 == NULL)
	{
		printf("File error.\n");
		exit(-1);
	}
	rewind(stdin);
	while (fscanf(fp2, "%[^,],%lf,%lf,%lf,%lf,%d/%d/%d\n", &info[i].id, &info[i].paidAmount, &info[i].monthlyAmount, &info[i].totalPayBackAmount, &info[i].dueAmount, &info[i].date.year, &info[i].date.month, &info[i].date.day) != EOF)
	{
		i++;
	}
	fclose(fp2);
	count = i - 1;
	dueAmount = info[count].dueAmount;
	return dueAmount;
}

int forgottenPassword(int userType) {
	Student stud[100];
	Admin admin[5];
	int i = 0, numData, foundID = 0, count = 3, counter = 0;
	char id[15], question[100], ans[50];
	char answer[50];
	FILE* fptr;

	switch (userType) {
	case 1: {
		fptr = fopen("id_pass_name.txt", "r");
		if (fptr == NULL)
		{
			printf("Unable to open the file.\n");
			exit(-1);
		}

		while (fscanf(fptr, "%[^,],%[^,],%[^\n]\n", &stud[i].id, &stud[i].pass, &stud[i].name) != EOF) {
			if (!strcmp(stud[i].id, studID)) {
				counter = i;
				break;
			}
			i++;
		}
		fclose(fptr);
		fptr = fopen("SecurityQuestion.txt", "r");
		if (fptr == NULL)
		{
			printf("Unable to open the file.\n");
			exit(-1);
		}

		while (!feof(fptr)) {
			fscanf(fptr, "%[^,],%[^,],%[^\n]\n", &id, &question, &ans);
			i++;
			if (!strcmp(id, studID))
				break;

		}
		fclose(fptr);
		break;
	}
	case 0: {
		fptr = fopen("admin_id_pass.txt", "r");
		if (fptr == NULL)
		{
			printf("Unable to open the file.\n");
			exit(-1);
		}

		while (fscanf(fptr, "%[^,],%[^\n]\n", &admin[i].id, &admin[i].pass) != EOF) {
			if (!strcmp(admin[i].id, adminID)) {
				counter = i;
				break;
			}
			i++;
		}
		fclose(fptr);
		fptr = fopen("AdminSecurityQuestion.txt", "r");
		if (fptr == NULL)
		{
			printf("Unable to open the file.\n");
			exit(-1);
		}

		while (!feof(fptr)) {
			fscanf(fptr, "%[^,],%[^,],%[^\n]\n", &id, &question, &ans);
			i++;
			if (!strcmp(id, adminID))
				break;

		}
		fclose(fptr);
		break;
	}
	default:
		printf("something wrong\n");
		break;
	}




	for (i = 0; i < count; i++) {
		printf("\n  ----Answer your security question----\n\n");
		printf("  %s\n", question);
		printf("  Ans>>>> ");
		scanf("  %[^\n]", answer);
		if (!strcmp(answer, ans)) {
			printf("\n  Answer is correct.\n");
			if (userType == 0) {
				passwordDecryption(admin[counter].pass);
				printf("  Here is your password : %s\n", admin[counter].pass);
				break;
			}
			else {
				passwordDecryption(stud[counter].pass);
				printf("  Here is your password : %s\n", stud[counter].pass);
				break;
			}

		}
		if (i != 2)
			printf("  Invalid answer. Please try again\n\n");
		else printf("  Invalid answer for 3 times. Please try again later\n");
	}

	return 0;
}

int passwordDecryption(char password[]) {
	int i, len = 0;
	len = strlen(password);

	for (i = 0; i < len; i++) {
		password[i] -= 5;
	}
	return 0;
}

void getAmountLeft() {
	int status, monthlyAmountInt = 0, totalAmountInt = 0, rate = 0, rateAmount = 0, totalAmountwithRate = -1;
	double monthlyAmountDouble = 0;
	//char fileName[10] = "app", s1[7], file[9], txt[5] = ".txt";
	char str[200], str1[200], str2[200], str3[200], str4[200];
	/*FILE* fp1;
	sprintf(s1, "%d", orderNum);
	strcpy(file, fileName);
	strcat(file, s1);
	strcat(file, txt);*/
	status = chkPaymentStatus();
	switch (status) {
	case 1: {
		totalAmountInt = getAmount(&totalAmountwithRate);
		leftAmount = totalAmountwithRate / 100.0;
		break;
	}
	case 2: {
		monthlyAmountInt = getAllThreeAmount(&totalAmountwithRate);
		leftAmount = totalAmountwithRate / 100.0;
		break;
	}
	case 0: {
		leftAmount = 0;
		break;
	}
	default: {
		printf("  Something wrong.\n");
		break;
	}
	}
}

int getAdminID(char inputID[]) {
	strcpy(adminID, inputID);
	return 0;
}

void searchAdminID(char password[]) {
	Admin admin[5];
	int i = 0, numData, foundID = 0;
	char tempPass[20];
	char adminPassword[20];
	FILE* fptr;

	fptr = fopen("admin_id_pass.txt", "r");
	if (fptr == NULL)
	{
		printf("Unable to open the file.\n");
		exit(-1);
	}

	fscanf(fptr, "%[^,],%[^\n]\n", &admin[i].id, &admin[i].pass);
	while (!feof(fptr)) {
		i++;
		fscanf(fptr, "%[^,],%[^\n]\n", &admin[i].id, &admin[i].pass);
	}
	fclose(fptr);

	numData = i;

	for (i = 0; i <= numData; i++) {
		if (strcmp(admin[i].id, adminID) == 0) {
			strcpy(tempPass, admin[i].pass);
			passwordDecryption(tempPass);
		}
	}
	strcpy(password, tempPass);

}