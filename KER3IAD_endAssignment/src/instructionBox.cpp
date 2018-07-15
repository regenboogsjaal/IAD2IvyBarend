#include "instructionBox.h"



instructionBox::instructionBox()
{
}


instructionBox::~instructionBox()
{
}

void instructionBox::setup(int x, int y, string instruction, float stepID) 
{
	string databasePath = ofToDataPath("Recipes.db", true);
	db = new SQLite::Database(databasePath);
	instructionsQuery = new SQLite::Statement(*db, "SELECT ingredient, amount, measure FROM instructionIngredients WHERE stepID = ?");

	ingredientAmount = db->execAndGet("SELECT COUNT(stepID) AS ingredientAmount FROM instructionIngredients WHERE stepID =" + ofToString(stepID)).getInt();

	ingredientsFont.load("CaviarDreams.ttf", 12);
	number.load("CaviarDreams_Bold.ttf", 30);

	stepNumber = stepID;
	text = instruction;

	box.y = y;
	box.x = x;
	box.height = 200;// instructionText.getHeight();
	box.width = ofGetWidth() - 2 * box.x;

	instructionTextBox.x = box.x + margin;
	instructionTextBox.y = box.y + margin;
	instructionTextBox.width = (box.width/2) - 2 * margin;
	instructionTextBox.height = instructionText.getHeight();

	ingredientBox.x = box.x + margin;
	ingredientBox.y = box.y + (box.height / 2);
	ingredientBox.width = (box.width / 2) - 2 * margin;
	ingredientBox.height = (box.height / 2) + 2 * margin;

	instructionText = ofxParagraph(text, instructionTextBox.width);
	instructionText.setFont("CaviarDreams_Bold.ttf", 12);
	instructionText.setIndent(0);
	instructionText.setAlignment(ofxParagraph::ALIGN_CENTER);

	currentIngredient = 0;
}

void instructionBox::update()
{
	if (currentIngredient < ingredientAmount) {
		currentIngredient++;
		instructionsQuery->bind(1, stepNumber);
		while (instructionsQuery->executeStep())
		{
			ingredient = instructionsQuery->getColumn("ingredient").getText();
			ingredients.push_back(ingredient);

			measure = instructionsQuery->getColumn("measure").getText();
			measures.push_back(measure);

			amount = instructionsQuery->getColumn("amount").getDouble();
			amounts.push_back(amount);
		}
		instructionsQuery->reset();
	}
	
}

void instructionBox::draw()
{
	ofSetColor(ofColor(232, 198, 152));
	ofDrawRectangle(box);
	ofSetColor(ofColor::white);
	instructionText.draw(box.x, box.y + instructionText.getStringHeight(text));

	for (int i = 0; i < measures.size(); i++) {
		ofSetColor(ofColor::white);
		ingredientsFont.drawString(ingredients[i], 
									ingredientBox.x,(ingredientBox.y + ingredientsFont.stringHeight(ingredients[i])) 
									+ i * (ingredientsFont.stringHeight(ingredients[i]) + 2));
	}
}


