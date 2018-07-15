#pragma once

#include "ofMain.h"
#include "ofxParagraph.h"
#include "SQLiteCpp.h"

class instructionBox
{
public:
	instructionBox();
	~instructionBox();

	void setup(int x, int y, string instruction, float stepID);
	void update();
	void draw();

	SQLite::Database* db;
	SQLite::Statement* instructionsQuery;

	int margin = 10;

	string ingredient;
	double amount;
	string measure;

	vector<string> ingredients;
	vector<double> amounts;
	vector<string> measures;

	int currentIngredient;
	int ingredientAmount;
	int stepNumber;

	string text;
	ofRectangle box;
	ofRectangle photoBox;
	ofRectangle instructionTextBox;
	ofRectangle ingredientBox;


	ofxParagraph instructionText;
	ofTrueTypeFont ingredientsFont;
	ofTrueTypeFont number;

};

