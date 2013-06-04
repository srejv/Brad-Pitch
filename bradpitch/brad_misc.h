// brad_misc
// brad misc innehåller misc processering som inte passar någonstans.
// sista utväg för att få bort allt ur 

// Gör inget med signalen, skickar bara igenom den som den är.
void process_bypass(fixedp *input) {
	return;
}

// HARD CLIPPING
void process_HARDCLIP(fixedp *pInput) {
	int i;
	for(i = 0; i < PROCESS_SIZE; i++) {
		if(pInput[i] >= AUDIOMAX) pInput[i] = AUDIOMAX-1;
		else if(pInput[i] <= AUDIOMIN) pInput[i] = AUDIOMIN+1;
	}

	return;
}