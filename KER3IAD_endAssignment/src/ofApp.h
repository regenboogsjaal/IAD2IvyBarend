#pragma once

#include "ofMain.h"
#include "SQLiteCpp.h"
#include "instructionBox.h"
#include "ofxSmartFont.h"
#include "ofxParagraph.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		void notificationBox(int x, int y, string message);
		bool notificationExpired();

		bool screen1;
		bool screen2;
		bool screen3;

		//database
		SQLite::Database* db;
		SQLite::Statement* recipeQuery;
		SQLite::Statement* instructionQuery;

		//screen1

		int lineY;
		int ingredientX;
		int amountX;
		int measureX;

		string ingredient;
		double amount;
		string measure;

		vector<string> ingredients;
		vector<double> amounts;
		vector<string> measures;

		int listEdge_left;
		int listEdge_right;

		ofTrueTypeFont shopFont;

		int currentIngredientCode;
		int selectedIngredientCode;
		int maxIngredientCode;
		int minIngredientCode;

		//_____________________________

		//screen 2
		int instructionAmount;

		instructionBox instruction;
		vector<instructionBox> instructions;

		int nextButtonX;
		int nextButtonY;
		int nextButtonR;

		int prevButtonX;



		//____________________________

		//Screen 3

		int minuteMultiplier;

		ofRectangle totalBar;
		ofRectangle timeBar;

		ofRectangle ovenBar_prep;
		ofRectangle ovenBar_preheat;
		ofRectangle ovenBar_prep_cook;
		ofRectangle ovenBar_cook;

		int _ovenBar_prep;
		int _ovenBar_preheat;
		int _ovenBar_prep_cook;
		int _ovenBar_cook;

		ofRectangle skilletBar_null;
		ofRectangle skilletBar_prep;
		ofRectangle skilletBar_glaze;
		ofRectangle skilletBar_simmer;

		int _skilletBar_null;
		int _skilletBar_prep;
		int _skilletBar_glaze;
		int _skilletBar_simmer;

		ofRectangle potBar_null;
		ofRectangle potBar_prep;
		ofRectangle potBar_water;
		ofRectangle potBar_prep_pasta;
		ofRectangle potBar_pasta;

		int _potBar_null;
		int _potBar_prep;
		int _potBar_water;
		int _potBar_prep_pasta;
		int _potBar_pasta;

		float timeInSeconds;
		int totalCookingTime;
		bool cooking;

		ofRectangle backgroundBox_3;
		ofRectangle iconBox;

		int marginY;

		int iconX;
		int clockY;
		int ovenY;
		int skilletY;
		int potY;

		ofImage icon_clock;
		ofImage icon_oven;
		ofImage icon_pot;
		ofImage icon_skillet;

		float clockRatio;
		float ovenRatio;
		float skilletRatio;
		float potRatio;

		int clockW;
		int clockH;
		int ovenW;
		int ovenH;
		int skilletW;
		int skilletH;
		int potW;
		int potH;

		//____________________________

		//Buttons

		int buttonNext_x;
		int buttonNext_y;
		int buttonNext_width;
		int buttonNext_height;

		int buttonPrev_x;
		int buttonPrev_y;
		int buttonPrev_width;
		int buttonPrev_height;

		ofImage buttonNext;
		ofImage buttonPrev;

		//___________________________
		
		//int instructionsDrawn;
		
		//int desiredY;
	
		//bool shouldRemove;
		
		
		//vector<string> instructionStrings;

		//instructionBox instruction;

		

		

		


		
		//int minutes;
		
		//int totalOvenTime;

		//int startPreheatOvenTime;
		//int startBakingOvenTime;

		//float value;
		
		//float lerpMinute;
		

		//int margin = 50;
		//int textMargin = 5;

		

		

		//ofRectangle buttonLeft;
		//ofRectangle buttonRight;

		

		


		//float secondToSize = 0.385;
		
		
		

		

		

		

		//ofImage tri_left;
		//ofImage tri_right;

		

		
		

		
		
		

		//int iconMargin;

		

		//int iconSize;
		
		

		//int scaleRatio = 0.5;

		
		

};
