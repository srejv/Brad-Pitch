// brad_misc
// brad misc inneh�ller misc processering som inte passar n�gonstans.
// sista utv�g f�r att f� bort allt ur 

// G�r inget med signalen, skickar bara igenom den som den �r.
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