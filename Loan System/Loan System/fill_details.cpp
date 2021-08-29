#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#pragma warning(disable: 4996)

bool chkPhone(char phoneNum[]) {
	int i, count;
	count = 0;

	for (i = 0; i < strlen(phoneNum); i++) {
		count++;
		if (isdigit(phoneNum[i]) == 0) {
			return false;
			break;
		}
	}

	if (count == 10 || count == 11) {
		return true;
	}
	return false;
}

bool chkIC(char ic[]) {
	int i, count;

	count = 0;

	for (i = 0; i < strlen(ic); i++) {
		count++;
		if (isdigit(ic[i]) == 0) {
			return false;
			break;
		}
	}

	if (count == 12) {
		return true;
	}
	return false;
}

int fillLevelofStudy(char levelofStudy[]) {
	char level, chkLevel;

	printf("\n\n\n\n\t\t\t<< Acedemic Details of Applicant >>\n");
	printf(" ================================================================================\n\n");

	do {
		printf("  Enter level of study (Diploma - D / Degree - R) : ");
		rewind(stdin);
		scanf("%c", &level);
		chkLevel = getchar();
		if (chkLevel != '\n' && chkLevel != EOF) {
			printf("  Invalid input, please enter again.\n\n");
		}
		else {
			if (toupper(level) != 'D' && toupper(level) != 'R') {
				printf("  Invalid input, please enter again.\n\n");
			}
			else if (toupper(level) == 'D') {
				strcpy(levelofStudy, "Diploma");
			}
			else if (toupper(level) == 'R') {
				strcpy(levelofStudy, "Degree");
			}
		}
	} while ((chkLevel != '\n' && chkLevel != EOF) || (toupper(level) != 'D' && toupper(level) != 'R'));

	return 0;
}

int fillGuardianDetail(char name[2][50], char ic[2][15], char phoneNum[2][15], int salary[]) {
	int i;

	printf("\n\n\n\n\t\t\t<< Guardian Details of Applicant >>\n");
	printf(" ================================================================================\n\n");

	for (i = 0; i < 2; i++) {
		printf("  Enter name of Guardian %d                               : ", i + 1);
		rewind(stdin);
		scanf("%[^\n]", name[i]);
		do {
			printf("  Enter IC number of Guardian %d (without \'-'\)            : ", i + 1);
			rewind(stdin);
			scanf("%[^\n]", ic[i]);
			if (chkIC(ic[i]) == 0) {
				printf("  Invalid input, please check that only numbers are allowed.\n\n");
			}
			else {
				do {
					printf("  Enter contact phone number of Guardian %d (without \'-\') : ", i + 1);
					rewind(stdin);
					scanf("%[^\n]", phoneNum[i]);
					if ((chkPhone(phoneNum[i]) == 0)) {
						printf("  Invalid input, please check that only numbers are allowed.\n\n");
					}
					else {
						do {
							printf("  Enter salary of Guardian %d per month (0 if unemployed) : ", i + 1);
							rewind(stdin);
							scanf("%d", &salary[i]);
							if (salary[i] < 0) {
								printf("  Invalid input, please try again.\n\n");
							}
							printf("\n\n");
						} while (salary[i] < 0);
					}
				} while (chkPhone(phoneNum[i]) == 0);
			}
		} while (chkIC(ic[i]) == 0);
	};	

	return 0;
}

int checkWorkingStatus(char chkWork, char name[], char phoneNum[], char companyName[], char position[], int* salary) {
	if (toupper(chkWork) == 'Y') {
		printf("\n\n\n\n\t\t\t<< Working Details of Applicant >>\n");
		printf(" ================================================================================\n\n");

		printf("  Enter name of employer                               : ");
		rewind(stdin);
		scanf("%[^\n]", name);

		do {
			printf("  Enter contact phone number of employer (without \'-\') : ");
			rewind(stdin);
			scanf("%[^\n]", phoneNum);
			if (chkPhone(phoneNum) == 0) {
				printf("  Invalid input, please check that only numbers are allowed.\n\n");
			}
			else {
				printf("  Enter name of company working with                   : ");
				rewind(stdin);
				scanf("%[^\n]", companyName);
				printf("  Enter position                                       : ");
				rewind(stdin);
				scanf("%[^\n]", position);
				do {
					printf("  Enter salary per month                               : ");
					rewind(stdin);
					scanf("%d", &*salary);
					if (*salary < 1100) {
						printf("  Invalid input, please try again.\n\n");
					}
				} while (*salary < 1100);
			}
		} while (chkPhone(phoneNum) == 0);
	}
	return 0;
}

int fillPersonalDetails(char phoneNum[], char email[], char* chkWork) {
	char chkChar;

	printf("\n\t\t\t<< Personal Details of Applicant >>\n");
	printf(" ================================================================================\n\n");

	printf("  Enter contact email                      : ");
	rewind(stdin);
	scanf("%[^\n]", email);

	do {
		printf("  Enter contact phone number (without \'-\') : ");
		rewind(stdin);
		scanf("%[^\n]", phoneNum);
		if (chkPhone(phoneNum) == 0) {
			printf("  Invalid input, please check that only numbers are allowed.\n\n");
		}
		else {
			do {
				printf("  Is applicant still working full time (y/n) ? ");
				rewind(stdin);
				scanf("%c", &*chkWork);
				chkChar = getchar();
				if (chkChar != '\n' && chkChar != EOF) {
					printf("  Invalid input, please enter again.\n\n");
				}
				else {
					if (toupper(*chkWork) != 'Y' && toupper(*chkWork) != 'N') {
						printf("  Invalid input, please enter again.\n\n");
					}
				}
			} while ((chkChar != '\n' && chkChar != EOF) || (toupper(*chkWork) != 'Y' && toupper(*chkWork) != 'N'));
		}
	} while (chkPhone(phoneNum) == 0);

	return 0;
}