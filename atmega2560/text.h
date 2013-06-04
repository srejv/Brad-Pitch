/*
 * text.h
 *
 * Created: 3/19/2013 10:28:13 AM
 *  Author: Martin Andersson
 */ 

/* Kommentera ut de strängar som inte ska användas för att
 * kunna enklare lägga tillbaka fler i framtiden. */

const char text_effect01[] PROGMEM = "Delay:         %s\0";
const char text_effect02[] PROGMEM = "Distortion:    %s\0";
const char text_effect03[] PROGMEM = "Vibrato:       %s\0";
const char text_effect04[] PROGMEM = "Flanger:       %s\0";
const char text_effect05[] PROGMEM = "Chorus:        %s\0";
const char text_effect06[] PROGMEM = "Pitch shift:   %s\0";
const char text_effect07[] PROGMEM = "Equalizer:     %s\0";
const char text_effect08[] PROGMEM = "Tremolo:       %s\0";
const char text_effect09[] PROGMEM = "Reverb:        %s\0";

PGM_P const text_effect_table[] PROGMEM =
{
	text_effect01,
	text_effect02,
	text_effect03,
	text_effect04,
	text_effect05,
	text_effect06,
	text_effect07,
	text_effect08,
	text_effect09
};


/* Delay. (Effekt 1) */
const char text_parameter0101[] PROGMEM = "Mix(%%):        %d\0";
const char text_parameter0102[] PROGMEM = "Time(ms):      %d\0";
const char text_parameter0103[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0104[] PROGMEM = "Fdb.Filter(0-4):%d\0";
const char text_parameter0105[] PROGMEM = "Filter fc(Hz):  %d\0";
const char text_parameter0106[] PROGMEM = "DriveInGain(%%):%d\0";
const char text_parameter0107[] PROGMEM = "Atk. Thres(%%): %d\0";
/* Distortion. (Effekt 2) */
const char text_parameter0201[] PROGMEM = "Mix(%%):        %d\0";
const char text_parameter0202[] PROGMEM = "Gain(%%):       %d\0";
const char text_parameter0203[] PROGMEM = "Level 1(%%):    %d\0";
const char text_parameter0204[] PROGMEM = "Level 2(%%):    %d\0";
const char text_parameter0205[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0206[] PROGMEM = "Type(0-3):     %d\0";
const char text_parameter0207[] PROGMEM = "parameter 7:   %d\0";
/* Vibrato. (Effekt 3) */
const char text_parameter0301[] PROGMEM = "Mix(100/0):    %d\0";
const char text_parameter0302[] PROGMEM = "Mod. freq(cHz):%d\0";
const char text_parameter0303[] PROGMEM = "Mod. dept(%%):  %d\0";
const char text_parameter0304[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0305[] PROGMEM = "parameter 5:   %d\0";
const char text_parameter0306[] PROGMEM = "parameter 6:   %d\0";
const char text_parameter0307[] PROGMEM = "parameter 7:   %d\0";
/* Flanger. (Effekt 4) */
const char text_parameter0401[] PROGMEM = "Mix(50/50)     %d\0";
const char text_parameter0402[] PROGMEM = "Mod. freq(cHz):%d\0";
const char text_parameter0403[] PROGMEM = "Mod. dept(%%):  %d\0";
const char text_parameter0404[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0405[] PROGMEM = "parameter 5:   %d\0";
const char text_parameter0406[] PROGMEM = "parameter 6:   %d\0";
const char text_parameter0407[] PROGMEM = "parameter 7:   %d\0";
/* Chorus. (Effekt 5) */
const char text_parameter0501[] PROGMEM = "Mix(50/50):    %d\0";
const char text_parameter0502[] PROGMEM = "Mod. freq(cHz):%d\0";
const char text_parameter0503[] PROGMEM = "Mod. dept(%%):  %d\0";
const char text_parameter0504[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0505[] PROGMEM = "Offset(ms):    %d\0";
const char text_parameter0506[] PROGMEM = "parameter 6:   %d\0";
const char text_parameter0507[] PROGMEM = "parameter 7:   %d\0";
/* Pitch shift (Effekt 6) */
const char text_parameter0601[] PROGMEM = "Mix(%%):        %d\0";
const char text_parameter0602[] PROGMEM = "Pitch step:    %d\0";
const char text_parameter0603[] PROGMEM = "Pitch cent.:   %d\0";
const char text_parameter0604[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0605[] PROGMEM = "Gain(%%):       %d\0";
const char text_parameter0606[] PROGMEM = "Offset(ms):    %d\0";
const char text_parameter0607[] PROGMEM = "parameter 7:   %d\0";
/* Equalizer (Effekt 7) */
const char text_parameter0701[] PROGMEM = "Mix(%%):        %d\0";
const char text_parameter0702[] PROGMEM = "Low gain(%%):   %d\0";
const char text_parameter0703[] PROGMEM = "Mid gain(%%):   %d\0";
const char text_parameter0704[] PROGMEM = "High gain(%%):  %d\0";
const char text_parameter0705[] PROGMEM = "Low freq(Hz):  %d\0";
const char text_parameter0706[] PROGMEM = "Mid freq(Hz):  %d\0";
const char text_parameter0707[] PROGMEM = "High freq(kHz):%d\0";
/* Tremolo. (Effekt 8) */
const char text_parameter0801[] PROGMEM = "Mix(%%):        %d\0";
const char text_parameter0802[] PROGMEM = "Mod. freq(cHz):%d\0";
const char text_parameter0803[] PROGMEM = "Mod. dept(%%):  %d\0";
const char text_parameter0804[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0805[] PROGMEM = "parameter 5:   %d\0";
const char text_parameter0806[] PROGMEM = "parameter 6:   %d\0";
const char text_parameter0807[] PROGMEM = "parameter 7:   %d\0";
/* Reverb. (Effekt 9) */
const char text_parameter0901[] PROGMEM = "Mix(%%):        %d\0";
const char text_parameter0902[] PROGMEM = "Feedback(%%):   %d\0";
const char text_parameter0903[] PROGMEM = "FreqC(cHz):    %d\0";
const char text_parameter0904[] PROGMEM = "Time(ms):      %d\0";
const char text_parameter0905[] PROGMEM = "parameter 5:   %d\0";
const char text_parameter0906[] PROGMEM = "parameter 6:   %d\0";
const char text_parameter0907[] PROGMEM = "parameter 7:   %d\0";

PGM_P const text_parameter_table[] PROGMEM =
{		
	text_parameter0101,
	text_parameter0102,
	text_parameter0103,
	text_parameter0104,
	text_parameter0105,
	text_parameter0106,
	text_parameter0107,
	
	text_parameter0201,
	text_parameter0202,
	text_parameter0203,
	text_parameter0204,
	text_parameter0205,
	text_parameter0206,
	text_parameter0207,
	
	text_parameter0301,
	text_parameter0302,
	text_parameter0303,
	text_parameter0304,
	text_parameter0305,
	text_parameter0306,
	text_parameter0307,
	
	text_parameter0401,
	text_parameter0402,
	text_parameter0403,
	text_parameter0404,
	text_parameter0405,
	text_parameter0406,
	text_parameter0407,
	
	text_parameter0501,
	text_parameter0502,
	text_parameter0503,
	text_parameter0504,
	text_parameter0505,
	text_parameter0506,
	text_parameter0507,
	
	text_parameter0601,
	text_parameter0602,
	text_parameter0603,
	text_parameter0604,
	text_parameter0605,
	text_parameter0606,
	text_parameter0607,
	
	text_parameter0701,
	text_parameter0702,
	text_parameter0703,
	text_parameter0704,
	text_parameter0705,
	text_parameter0706,
	text_parameter0707,
	
	text_parameter0801,
	text_parameter0802,
	text_parameter0803,
	text_parameter0804,
	text_parameter0805,
	text_parameter0806,
	text_parameter0807,
	
	text_parameter0901,
	text_parameter0902,
	text_parameter0903,
	text_parameter0904,
	text_parameter0905,
	text_parameter0906,
	text_parameter0907,
};