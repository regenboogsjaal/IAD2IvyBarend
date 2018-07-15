#include "ofApp.h"

void ofApp::setup(){

	ofBackground(ofColor::white);
	shopFont.load("CaviarDreams.ttf", 20);
	string databasePath = ofToDataPath("Recipes.db", true);
	db = new SQLite::Database(databasePath);

	
	recipeQuery = new SQLite::Statement(*db, "SELECT ingredient, SUM(amount) AS total_amount, measure FROM meatBallMarinara WHERE ingredientCode = ? GROUP BY ingredientCode");
	instructionQuery = new SQLite::Statement(*db, "SELECT instruction, timerBool, time FROM instructions WHERE stepNumber = ?");

	cooking = false;

	screen1 = true;
	screen2 = false;
	screen3 = false;
	
	minuteMultiplier = 1;

	totalCookingTime = db->execAndGet("SELECT time FROM CookingTimes WHERE tag = 'total'").getInt() * minuteMultiplier;//40, now in seconds, should be *60

	//cookingtimes for oven
	_ovenBar_prep = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'oven_prep'").getInt() * minuteMultiplier;
	_ovenBar_preheat = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'oven_preheat'").getInt() * minuteMultiplier;
	_ovenBar_prep_cook = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'oven_prep_mb'").getInt() * minuteMultiplier;
	_ovenBar_cook = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'oven_cook_mb'").getInt() * minuteMultiplier;
	
	//cookingtimes for skillet
	_skilletBar_prep = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'skillet_prep'").getInt() * minuteMultiplier;
	_skilletBar_glaze = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'skillet_glaze'").getInt() * minuteMultiplier;
	_skilletBar_simmer = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'skillet_simmer'").getInt() * minuteMultiplier;
	_skilletBar_null = totalCookingTime - (_skilletBar_prep + _skilletBar_glaze + _skilletBar_simmer);

	//cookingtimes for pot
	_potBar_prep = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'pot_prep'").getInt() * minuteMultiplier;
	_potBar_water = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'pot_boil_water'").getInt() * minuteMultiplier;
	_potBar_prep_pasta = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'pot_prep_pasta'").getInt() * minuteMultiplier;
	_potBar_pasta = db->execAndGet("SELECT time FROM CookingTimes WHERE action = 'pot_boil_pasta'").getInt() * minuteMultiplier;
	_potBar_null = totalCookingTime - (_potBar_prep + _potBar_water + _potBar_prep_pasta + _potBar_pasta);

	//ingredientcodes
	maxIngredientCode = db->execAndGet("SELECT MAX(ingredientCode) FROM meatballMarinara").getInt();
	minIngredientCode = db->execAndGet("SELECT MIN(ingredientCode) FROM meatballMarinara").getInt();

	currentIngredientCode = 0;

	instructionAmount = db->execAndGet("SELECT MAX(stepNumber) FROM instructions").getInt();

	//buttons and icons load
	buttonNext.load("images/cooky_buttonNext.png");
	buttonPrev.load("images/cooky_buttonPrev.png");
	icon_clock.load("images/clock.jpg");
	icon_oven.load("images/oven.jpg");
	icon_pot.load("images/pot.jpg");
	icon_skillet.load("images/skillet.jpg");

	//get icon sizes
	clockW = icon_clock.getWidth();
	clockH = icon_clock.getHeight();
	ovenW = icon_oven.getWidth();
	ovenH = icon_oven.getHeight();
	skilletW = icon_skillet.getWidth();
	skilletH = icon_skillet.getHeight();
	potW = icon_pot.getWidth();
	potH = icon_pot.getHeight();

	//scale ratio
	clockRatio = 200 / icon_clock.getWidth();
	ovenRatio = 200 / icon_oven.getWidth();
	skilletRatio = 200 / icon_skillet.getWidth();
	potRatio = 200 / icon_pot.getWidth();

}

void ofApp::update(){

	//draw navigation buttons
	buttonNext_x = ofGetWidth() - 120;
	buttonNext_y = ofGetHeight() / 2;
	buttonNext_width = 100;
	buttonNext_height = 100;

	buttonPrev_x = 20;
	buttonPrev_y = ofGetHeight() / 2;
	buttonPrev_width = 100;
	buttonPrev_height = 100;

	//edge from buttons plus margin
	listEdge_left = buttonPrev_x + buttonPrev_width + 20;
	listEdge_right = buttonNext_x - 20;

	if (screen1)
	{
		//starting Y pos for lines of ingredients
		lineY = 100;
	}

	if (screen2) 
	{
		//update instructionbox
		for (int i = 0; i < instructions.size(); i++) {
			instructions[i].update();
		}

	}

	if (screen3)
	{
		//function for accurately displaying passing of time during cooking
		//check elapsed time in seconds, compare to total cooking time, if reached, reset
		if (cooking == true) {
			timeInSeconds = ofGetElapsedTimef();
			if (timeInSeconds > totalCookingTime) {
				cooking = false;
				ofResetElapsedTimeCounter();
			}
		}
	}
}

void ofApp::draw(){

	if (screen1) 
	{
		if (currentIngredientCode < maxIngredientCode)
		{
			currentIngredientCode++;
			//check max amount of unique ingredients
			//add them to vector per column
			recipeQuery->bind(1, currentIngredientCode);
			while (recipeQuery->executeStep())
			{
				ingredient = recipeQuery->getColumn("ingredient").getText();
				ingredients.push_back(ingredient);

				measure = recipeQuery->getColumn("measure").getText();
				measures.push_back(measure);

				amount = recipeQuery->getColumn("total_amount").getDouble();
				amounts.push_back(amount);
			}
			recipeQuery->reset();
		}

		ofLog() << "ingredientX = " << ingredientX << " | measureX = " << measureX << " | amountX = " << amountX << endl;
		//for every vector element, draw ingredient on screen
		for (int i = 0; i < ingredients.size(); i++)
		{
			ingredientX = listEdge_left + 50;
			ofSetColor(ofColor::darkGrey);
			ofDrawRectangle(listEdge_left, lineY + i * 40, listEdge_right - listEdge_left, 1);
			ofSetColor(ofColor::black);
			shopFont.drawString(ingredients[i], ingredientX, lineY + i * 40);
		}

		for (int i = 0; i < measures.size(); i++) 
		{
			measureX = listEdge_right - 50 - shopFont.stringWidth(measures[i]);
			ofSetColor(ofColor::black);
			shopFont.drawString(measures[i], measureX, lineY + i * 40);
		}

		for (int i = 0; i < amounts.size(); i++)
		{
			amountX = (listEdge_right - 50 - shopFont.stringWidth(measures[i])) - 10 - shopFont.stringWidth(ofToString(amounts[i]));
			ofSetColor(ofColor::black);
			shopFont.drawString(ofToString(amounts[i]), amountX, lineY + i * 40);
		}

		ofSetColor(ofColor::white);
		buttonNext.draw(buttonNext_x, buttonNext_y, buttonNext_width, buttonNext_height);
	}

	if (screen2) 
	{
		
		buttonNext.draw(buttonNext_x, buttonNext_y, buttonNext_width, buttonNext_height);
		buttonPrev.draw(buttonPrev_x, buttonPrev_y, buttonPrev_width, buttonPrev_height);

		prevButtonX = 100;
		nextButtonX = ofGetWidth() - 100;
		nextButtonY = ofGetHeight() - 100;
		nextButtonR = 50;
		ofSetColor(ofColor::green);
		ofDrawCircle(nextButtonX, nextButtonY, nextButtonR);
		ofDrawCircle(prevButtonX, nextButtonY, nextButtonR);
		ofSetColor(ofColor::white);

		//get instructions from database and put them in class instructionBox
		string instructionText = db->execAndGet("SELECT instruction FROM instructions WHERE stepNumber =" + ofToString(instructions.size()));
		instruction.setup(listEdge_left, 20, instructionText, instructions.size());

		for (int i = 0; i < instructions.size(); i++) {
			instructions[i].draw();
		}
	}

	if (screen3) 
	{
		backgroundBox_3.x = 140;
		backgroundBox_3.y = 0;
		backgroundBox_3.width = ofGetWidth() - 140;
		backgroundBox_3.height = ofGetHeight();

		ofSetColor(ofColor(244, 228, 205)); //pale pink
		ofDrawRectangle(backgroundBox_3);

		iconBox.x = ofGetWidth() - 300;
		iconBox.y = 0;
		iconBox.width = 300;
		iconBox.height = ofGetHeight();

		ofSetColor(ofColor(232, 198, 152)); //pale orange
		ofDrawRectangle(iconBox);

		marginY = ((ofGetHeight() / 4) - 200) / 2;

		iconX = iconBox.x + 50;
		clockY = marginY;
		ovenY = (3 * marginY) + 200;
		skilletY = (5 * marginY) + 400;
		potY = (7 * marginY) + 600;

		ofSetColor(ofColor::white);

		icon_clock.resize(clockW * clockRatio, clockH * clockRatio);
		icon_clock.draw(iconX, clockY);

		icon_oven.resize(ovenW * ovenRatio, ovenH * ovenRatio);
		icon_oven.draw(iconX, ovenY);

		icon_skillet.resize(skilletW * skilletRatio, skilletH * ovenRatio);
		icon_skillet.draw(iconX, skilletY);

		icon_pot.resize(potW * potRatio, potH * potRatio);
		icon_pot.draw(iconX, potY);

		
		//TIMEBARS________________________________________________________________
		
		timeBar.width = ofMap(timeInSeconds, 0, totalCookingTime, 0, totalBar.width);
		

		totalBar.x = backgroundBox_3.x;
		totalBar.y = clockY + 50;
		totalBar.width = (ofGetWidth() - totalBar.x) - iconBox.width;
		totalBar.height = 100;
		ofSetColor(ofColor(203, 136, 47)); //dark orange
		ofDrawRectangle(totalBar);

		ofSetColor(ofColor(244, 228, 205)); // pale pink
		ofDrawRectangle(totalBar.x, totalBar.y, timeBar.width, totalBar.height);

		//OVEN____________________________________________________________________

		ovenBar_prep.x = totalBar.x;
		ovenBar_prep.y = ovenY + 50;
		ovenBar_prep.width = ofMap(_ovenBar_prep, 0, totalCookingTime, 0, totalBar.width);
		ovenBar_prep.height = totalBar.height;
		ofSetColor(ofColor(205, 216, 140)); //pale green
		ofDrawRectangle(ovenBar_prep);

		ovenBar_preheat.x = ovenBar_prep.x + ovenBar_prep.width;
		ovenBar_preheat.y = ovenBar_prep.y;
		ovenBar_preheat.width = ofMap(_ovenBar_preheat, 0, totalCookingTime, 0, totalBar.width);
		ovenBar_preheat.height = totalBar.height;
		ofSetColor(ofColor(192,215,45)); //light green
		ofDrawRectangle(ovenBar_preheat);


		ovenBar_prep_cook.x = ovenBar_preheat.x + ovenBar_preheat.width;
		ovenBar_prep_cook.y = ovenBar_prep.y;
		ovenBar_prep_cook.width = ofMap(_ovenBar_prep_cook, 0, totalCookingTime, 0, totalBar.width);
		ovenBar_prep_cook.height = totalBar.height;
		ofSetColor(ofColor(205, 216, 140)); // pale green
		ofDrawRectangle(ovenBar_prep_cook);


		ovenBar_cook.x = ovenBar_prep_cook.x + ovenBar_prep_cook.width;
		ovenBar_cook.y = ovenBar_prep.y;
		ovenBar_cook.width = ofMap(_ovenBar_cook, 0, totalCookingTime, 0, totalBar.width);
		ovenBar_cook.height = totalBar.height;
		ofSetColor(ofColor(164, 184, 35)); //dark, base green
		ofDrawRectangle(ovenBar_cook);


		ofSetColor(ofColor(244, 228, 205)); // pale pink
		ofDrawRectangle(ovenBar_prep.x, ovenBar_prep.y, timeBar.width, totalBar.height);

		//SKILLET____________________________________________________________________________________

		skilletBar_null.x = totalBar.x;
		skilletBar_null.y = skilletY + 50;
		skilletBar_null.width = ofMap(_skilletBar_null, 0, totalCookingTime, 0, totalBar.width);
		skilletBar_null.height = totalBar.height;
		ofSetColor(ofColor(244, 228, 205)); // pale pink
		ofDrawRectangle(skilletBar_null);

		skilletBar_prep.x = skilletBar_null.x + skilletBar_null.width;
		skilletBar_prep.y = skilletBar_null.y;
		skilletBar_prep.width = ofMap(_skilletBar_prep, 0, totalCookingTime, 0, totalBar.width);
		skilletBar_prep.height = totalBar.height;
		ofSetColor(ofColor(211,164,138)); //pale red
		ofDrawRectangle(skilletBar_prep);


		skilletBar_glaze.x = skilletBar_prep.x + skilletBar_prep.width;
		skilletBar_glaze.y = skilletBar_null.y;
		skilletBar_glaze.width = ofMap(_skilletBar_glaze, 0, totalCookingTime, 0, totalBar.width);
		skilletBar_glaze.height = totalBar.height;
		ofSetColor(ofColor(241, 114, 47)); //light red
		ofDrawRectangle(skilletBar_glaze);

		

		skilletBar_simmer.x = skilletBar_glaze.x + skilletBar_glaze.width;
		skilletBar_simmer.y = skilletBar_null.y;
		skilletBar_simmer.width = ofMap(_skilletBar_simmer, 0, totalCookingTime, 0, totalBar.width);
		skilletBar_simmer.height = totalBar.height;
		ofSetColor(ofColor(215, 105, 45)); //dark, base red
		ofDrawRectangle(skilletBar_simmer);


		ofSetColor(ofColor(244, 228, 205)); // pale pink
		ofDrawRectangle(skilletBar_null.x, skilletBar_null.y, timeBar.width, totalBar.height);

		//POT___________________________________________________________________________________

		potBar_null.x = totalBar.x;
		potBar_null.y = potY + 50;
		potBar_null.width = ofMap(_potBar_null, 0, totalCookingTime, 0, totalBar.width);
		potBar_null.height = totalBar.height;
		ofSetColor(ofColor(244, 228, 205)); // pale pink
		ofDrawRectangle(potBar_null);

		potBar_prep.x = potBar_null.x + potBar_null.width;
		potBar_prep.y = potBar_null.y;
		potBar_prep.width = ofMap(_potBar_prep, 0, totalCookingTime, 0, totalBar.width);
		potBar_prep.height = totalBar.height;
		ofSetColor(ofColor(192, 213, 214)); //pale blue
		ofDrawRectangle(potBar_prep);


		potBar_water.x = potBar_prep.x + potBar_prep.width;
		potBar_water.y = potBar_null.y;
		potBar_water.width = ofMap(_potBar_water, 0, totalCookingTime, 0, totalBar.width);
		potBar_water.height = totalBar.height;
		ofSetColor(ofColor(191, 233, 232)); //light blue
		ofDrawRectangle(potBar_water);
		

		potBar_prep_pasta.x = potBar_water.x + potBar_water.width;
		potBar_prep_pasta.y = potBar_null.y;
		potBar_prep_pasta.width = ofMap(_potBar_prep_pasta, 0, totalCookingTime, 0, totalBar.width);
		potBar_prep_pasta.height = totalBar.height;
		ofSetColor(ofColor(192, 213, 214)); //pale blue
		ofDrawRectangle(potBar_prep_pasta);


		potBar_pasta.x = potBar_prep_pasta.x + potBar_prep_pasta.width;
		potBar_pasta.y = potBar_null.y;
		potBar_pasta.width = ofMap(_potBar_pasta, 0, totalCookingTime, 0, totalBar.width);
		potBar_pasta.height = totalBar.height;
		ofSetColor(ofColor(152, 232, 231)); //base blue
		ofDrawRectangle(potBar_pasta);


		ofSetColor(ofColor(244, 228, 205)); // pale pink
		ofDrawRectangle(potBar_null.x, potBar_null.y, timeBar.width, totalBar.height);

		//NOTIFICATIONS

		if (timeBar.width + totalBar.x > ovenBar_prep.x && timeBar.width + totalBar.x < ovenBar_prep.x + ovenBar_prep.width && !notificationExpired())
		{
			notificationBox(ovenBar_prep.x + ((ovenBar_prep.width / 2) - 100), ovenBar_prep.y - 40, "warm up your oven to 220 degr. C");
		}

		if (timeBar.width + totalBar.x > ovenBar_prep_cook.x && timeBar.width + totalBar.x < ovenBar_prep_cook.x + ovenBar_prep_cook.width && !notificationExpired())
		{
			notificationBox(ovenBar_prep_cook.x + ((ovenBar_prep_cook.width / 2) - 100), ovenBar_prep_cook.y - 40, "put your meatballs in the oven for 18 minutes");
		}

		if (timeBar.width + totalBar.x > skilletBar_prep.x && timeBar.width + totalBar.x < skilletBar_prep.x + skilletBar_prep.width && !notificationExpired())
		{
			notificationBox(skilletBar_prep.x + ((skilletBar_prep.width / 2) - 100), skilletBar_prep.y - 40, "heat up your pan and 6 tbs of olive oil");
		}

		if (timeBar.width + totalBar.x > skilletBar_glaze.x && timeBar.width + totalBar.x < skilletBar_glaze.x + skilletBar_glaze.width && !notificationExpired())
		{
			notificationBox(skilletBar_glaze.x + ((skilletBar_glaze.width / 2) - 100), skilletBar_glaze.y - 40, "glaze your chopped onion");
		}

		if (timeBar.width + totalBar.x > skilletBar_simmer.x && timeBar.width + totalBar.x < skilletBar_simmer.x + skilletBar_simmer.width && !notificationExpired())
		{
			notificationBox(skilletBar_simmer.x + ((skilletBar_simmer.width / 2) - 100), skilletBar_simmer.y - 40, "add the sauce and white wine to the skillet and let simmer, stir occasionally");
		}

		if (timeBar.width + totalBar.x > potBar_prep.x && timeBar.width + totalBar.x < potBar_prep.x + potBar_prep.width && !notificationExpired())
		{
			notificationBox(potBar_prep.x + ((potBar_prep.width / 2) - 100), potBar_prep.y - 40, "put on water and add some olive oil and salt to it");
		}

		if (timeBar.width + totalBar.x > potBar_water.x && timeBar.width + totalBar.x < potBar_water.x + potBar_water.width && !notificationExpired())
		{
			notificationBox(potBar_water.x + ((potBar_water.width / 2) - 100), potBar_water.y - 40, "let the water come to a boil");
		}

		if (timeBar.width + totalBar.x > potBar_prep_pasta.x && timeBar.width + totalBar.x < potBar_prep_pasta.x + potBar_prep_pasta.width && !notificationExpired())
		{
			notificationBox(potBar_prep_pasta.x + ((potBar_prep_pasta.width / 2) - 100), potBar_prep_pasta.y - 40, "add pasta to the water");
		}




		buttonPrev.draw(buttonPrev_x, buttonPrev_y, buttonPrev_width, buttonPrev_height);
	}
}

void ofApp::notificationBox(int x, int y, string message) 
{
	int padding = 10;
	int boxWidth = 200;
	ofxParagraph instruction = ofxParagraph(message, boxWidth);
	instruction.setFont("CaviarDreams_Bold.ttf", 12);
	instruction.setIndent(0);
	int boxHeight = instruction.getHeight();
	int stringHeight = instruction.getStringHeight(message);

	ofSetColor(ofColor::darkGray);
	ofRectangle rect;
	rect.x = x - padding;
	rect.y = y - (stringHeight + padding);
	rect.width = boxWidth + padding;
	rect.height = boxHeight + padding;
	ofDrawRectangle(rect);
	ofDrawTriangle(rect.x + (rect.width/4), rect.y + rect.height,
				   rect.x + ((rect.width/4)*3), rect.y + rect.height,
				   rect.x + ((rect.width/4)*2), rect.y + rect.height + 20);

	instruction.setColor(ofColor::white);
	instruction.draw(x, y);
	ofSetColor(ofColor::white);
	
}

//doesnt work???
bool ofApp::notificationExpired()
{
	float time = ofGetElapsedTimef();
	if (ofGetElapsedTimef() > time + 10) {
		return true;
	}
	else {
		return false;
	}
}


void ofApp::keyPressed(int key){
	if (screen3) {
		if (key == ' ') {
			cooking = true;
			ofResetElapsedTimeCounter();
		}
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	//if buttonleft is pressed
	
	if (x > buttonPrev_x && x < buttonPrev_x + buttonPrev_width && y > buttonPrev_y && y < buttonPrev_y + buttonPrev_height)
	{
		if (screen2) {
			
			screen1 = true;
			screen2 = false;
		}
		else if (screen3) {
			
			screen2 = true;
			screen3 = false;
		}
	}
	//if buttonright is pressed
	if (x > buttonNext_x && x < buttonNext_x + buttonNext_width && y > buttonNext_y && y < buttonNext_y + buttonNext_height)
	{
		if (screen1) {
			
			screen2 = true;
			screen1 = false;
		}
		
		else if (screen2) {
			
			screen3 = true;
			screen2 = false;
		}
	}
	//if nextbutton screen2 is pressed
	if (x > nextButtonX - nextButtonR / 2 && x < nextButtonX + nextButtonR / 2 && y > nextButtonY - nextButtonR / 2 && y < nextButtonY + nextButtonR / 2)
	{
		if (instructions.size() < instructionAmount) {
			instructions.push_back(instruction);
		}
	}
	//if prevbutton screen2 is pressed
	if (x > prevButtonX - nextButtonR / 2 && x < prevButtonX + nextButtonR / 2 && y > nextButtonY - nextButtonR / 2 && y < nextButtonY + nextButtonR / 2)
	{
		if (instructions.size() > 1) {
			instructions.pop_back();
		}
	}
}