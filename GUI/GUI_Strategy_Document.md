# מסמך אסטרטגיה - מערכת ה-GUI של משחק Coup

## מבוא כללי

מערכת ה-GUI של משחק Coup היא ממשק גרפי מתקדם הבנוי על ספריית SFML (Simple and Fast Multimedia Library). המערכת מיועדת לספק חוויית משחק אינטואיטיבית ומודרנית, תוך התמקדות בעיצוב מינימליסטי ונקי שמדגיש את האלמנטים החשובים במשחק.

## רכיבי המערכת העיקריים

### 1. מבנה כללי

המערכת מורכבת משלושה מרכיבים עיקריים:
- **GameGUI** - המחלקה הראשית המנהלת את כל הממשק הגרפי
- **Game** - חיבור למנוע המשחק הבסיסי (Singleton Pattern)
- **SFML** - ספריית הגרפיקה והחלונות

### 2. עיצוב ויזואלי (Visual Design System)

#### NameSpace: VisualStyle
המערכת מבוססת על פלטת צבעים מודרנית המוגדרת ב-`VisualStyle`:

**צבעים ראשיים:**
- `PRIMARY_DARK` - רקע כהה (26, 32, 44)
- `PRIMARY_MEDIUM` - רקע בינוני (45, 55, 72)
- `PRIMARY_LIGHT` - רקע בהיר (74, 85, 104)

**צבעי הדגשה:**
- `ACCENT_BLUE` - כחול מודרני לכפתורים ראשיים
- `ACCENT_GREEN` - ירוק להצלחה
- `ACCENT_RED` - אדום לסכנה וביטול
- `ACCENT_YELLOW` - צהב להדגשות חשובות
- `ACCENT_PURPLE` - סגול לפעולות מיוחדות

#### פונקציות עיצוב מתקדמות:
- `createRoundedButton()` - יוצרת כפתורים עם מראה מודרני
- `createShadow()` - מוסיפה אפקט צל לאלמנטים חשובים

## זרימת המשחק (Game Flow)

### שלב 1: מסך הפתיחה (`showWelcomeScreen`)

**מטרה:** יצירת רושם ראשוני חיובי ומזמין

**רכיבים:**
- כותרת דרמטית "COUP" בגופן גדול
- כותרת משנה "Strategic Card Game"
- כפתור התחלה מעוצב עם צל ואפקטים
- הודעת ברכה אטמוספרית

**לוגיקה טכנית:**
- בדיקת רקע תמונה (אופציונלי)
- זיהוי קליק על כפתור ההתחלה
- מעבר למסך הבא

### שלב 2: הזנת שחקנים (`showPlayerInputScreen`)

**מטרה:** איסוף פרטי השחקנים בצורה ידידותית

**רכיבים עיקריים:**
- שדה הזנה מעוצב עם מסגרת כחולה
- רשימת שחקנים דינמית בעיצוב כרטיס
- כפתור התחלת משחק
- מסגרת שגיאות אדומה

**לוגיקה מורכבת:**
- בדיקת שמות כפולים
- הגבלת מספר שחקנים (2-6)
- מחיקה בעזרת Backspace
- אישור בעזרת Enter
- מנגנון שגיאות עם זמן תפוגה (3 שניות)

**אלגוריתם בדיקת תקינות:**
```
IF שם קיים:
    הצג שגיאה "שם כבר קיים"
ELSE IF מספר שחקנים == 6:
    הצג שגיאה "מקסימום 6 שחקנים"
ELSE:
    הוסף שחקן לרשימה
```

### שלב 3: חשיפת תפקידים (`showRoleRevealScreen`)

**מטרה:** יצירת מתח וציפייה תוך חשיפת תפקידים

**רכיבים:**
- כרטיס חשיפה דרמטי עם צל
- אנימציית הזנה משתמש-מונחית
- טיימר אוטומטי (2.5 שניות)
- הודעת אסטרטגיה עידודית

**לוגיקה חכמה:**
- ערבוב אקראי של תפקידים
- חיבור לGameFactory ליצירת שחקנים
- מתן זמן מתאים לקריאה והבנה

**עיצוב פסיכולוגי:**
- צבע זהב למרכז תשומת הלב
- צל כהה ליצירת עומק
- טקסט עידוד לבניית אסטרטגיה

### שלב 4: מסך המשחק הראשי (`runGameScreen`)

**מטרה:** הפעלת המשחק בממשק אינטואיטיבי וברור

#### פאנל מידע שמאלי:
**תכולה:**
- שם השחקן הנוכחי + "Turn"
- תפקיד השחקן
- מספר מטבעות (בצהוב מודגש)
- סטטוס סנקציה (ירוק/אדום)
- רשימת שחקנים פעילים

**עיצוב:**
- כרטיס עם צל ומסגרת כחולה
- היררכיית טקסט ברורה
- צבעים מבדילים לסטטוסים שונים

#### פאנל פעולות ימני:
**תכולה:**
- כותרת "Available Actions"
- כפתורי פעולה צבעוניים לפי סוג:
  - כחול: פעולות רגילות
  - ירוק: פעולות איסוף (Gather, Tax)
  - אדום: פעולות תוקפניות (Coup)
  - צהוב: פעולות מיוחדות (Bribe)

**לוגיקה דינמית:**
- קבלת פעולות זמינות מהשחקן הנוכחי
- יצירה דינמית של כפתורים
- זיהוי קליק והפעלת פעולה
- טיפול בשגיאות ברמת UI

#### אלגוריתם פעולות מתקדם:
```
FOR כל פעולה זמינה:
    IF פעולה דורשת מטרה (Coup, Arrest, Sanction, SpyOn):
        targetName = askForTargetPlayerName()
        IF targetName.empty():
            continue  // המשתמש ביטל
        
        targetPlayer = findPlayerByName(targetName)
        IF targetPlayer == nullptr:
            showErrorPopup("Player not found")
            continue
    
    TRY:
        SWITCH action:
            CASE Gather: currentPlayer->gather()
            CASE Tax: currentPlayer->tax()
            CASE Bribe: currentPlayer->bribe()
            CASE Invest: currentPlayer->invest()
            CASE Coup: currentPlayer->coup(*targetPlayer)
            CASE Arrest: currentPlayer->arrest(*targetPlayer)
            CASE Sanction: currentPlayer->sanction(*targetPlayer)
            CASE SpyOn: 
                currentPlayer->spyOn(*targetPlayer)
                viewPlayerCoinsPopup(targetPlayer->getName())
        
        turnEnded = true
        executedAction = action
        
        // בדיקת אפשרויות ביטול מכל השחקנים האחרים
        FOR כל שחקן p:
            IF p->isActive() AND p != currentPlayer AND p->canCancel(executedAction):
                answer = showCancelConfirmation(p->getName())
                IF answer == "yes":
                    TRY:
                        // יעד הביטול תלוי בפעולה המקורית
                        cancelTarget = (executedAction == Coup) ? *targetPlayer : *currentPlayer
                        p->cancel(cancelTarget)
                        break  // רק שחקן אחד יכול לבטל
                    CATCH cancel_error:
                        showErrorPopup(cancel_error.message)
        
    CATCH runtime_error:
        showErrorPopup(error.message)
        // התור לא מסתיים, השחקן יכול לנסות שוב
```

#### מנגנון בדיקת סיום מתקדם:
```
WHILE המשחק פעיל:
    // ספירה דינמית של שחקנים פעילים
    activePlayers = 0
    FOR player IN game.getPlayers():
        IF player->isActive():
            activePlayers++
    
    IF activePlayers <= 1:
        IF activePlayers == 1:
            // מציאת השחקן האחרון הפעיל
            FOR player IN game.getPlayers():
                IF player->isActive():
                    showWinnerScreen(player->getName())
                    RETURN
        showWinnerScreen("No one")  // מצב חריג - אף אחד לא נותר
        RETURN
    
    currentPlayer = game.getCurrentPlayer()
    
    // בדיקה אם השחקן הנוכחי לא פעיל (נוצע לאחרונה)
    IF !currentPlayer->isActive():
        TRY:
            game.nextTurn()  // מעבר לשחקן הבא הפעיל
            currentPlayer = game.getCurrentPlayer()
        CATCH error:
            BREAK  // שגיאה במעבר תור - יציאה בטוחה
    
    // המשך לעיבוד תור רגיל...
```

## חלונות עזר (Utility Windows)

### 1. בחירת מטרה (`askForTargetPlayerName`)

**מטרה:** בחירה מדויקת של מטרה לפעולות

**רכיבים:**
- חלון מודלי עם עיצוב מודרני
- שדה הזנה עם הדגשה כחולה
- כפתורי אישור וביטול מובחנים
- הודעות הנחיה ברורות

**חוויית משתמש:**
- הקלדה חופשית עם תצוגה מידית
- Enter לאישור, Escape לביטול
- זיהוי שגיאות והחזרת הודעות

### 2. אישור ביטול (`showCancelConfirmation`)

**מטרה:** החלטה מושכלת על ביטול פעולות

**עיצוב דרמטי:**
- כותרת אדומה מתריעה
- שם השחקן המבטל מודגש
- אזהרה על בלתי הפיכות ההחלטה
- כפתורים ברורים: "YES, CANCEL" ו-"NO, CONTINUE"

**קיצורי מקלדת:**
- Y = אישור ביטול
- N/Escape = המשך

### 3. דו"ח ריגול (`viewPlayerCoinsPopup`)

**מטרה:** הצגת מידע מטרה בסגנון ריגול

**עיצוב תימטי:**
- כותרת "SPY INTELLIGENCE REPORT"
- כרטיס מידע עם מסגרת זהובה
- הדגשת כמות המטבעות
- כפתור "GOT IT" לסגירה

### 4. הודעות שגיאה (`showErrorPopup`)

**מטרה:** העברת הודעות שגיאה ברורות ללא הפחדה

**גישה ידידותית:**
- כותרת אדומה אך לא מאיימת
- כרטיס שגיאה עם מסגרת אדומה
- טקסט ברור ומובן
- גלישת טקסט אוטומטית לשגיאות ארוכות

### פונקציות המרה טכניות מפורטות

#### המרת תפקידים (`roleToString`)
המרה מ-enum Role לstring לתצוגה באמצעות static map:
```cpp
static const std::map<Role, std::string> roleMap = {
    {Role::Governor, "Governor"}, {Role::Spy, "Spy"},
    {Role::Baron, "Baron"}, {Role::General, "General"},
    {Role::Judge, "Judge"}, {Role::Merchant, "Merchant"}
};
auto it = roleMap.find(role);
return (it != roleMap.end()) ? it->second : "Unknown";
```

**תוספות תפקיד-ספציפיות:**
- **Baron:** `ActionType::Invest` (אם יש 3+ מטבעות)
- **Spy:** `ActionType::SpyOn` (אם יש מטרות חוקיות)

## מנגנוני בטיחות וטיפול בשגיאות מתקדם

### 1. בדיקת משאבים ואתחול מערכת
**טעינת גופן:**
```cpp
if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    throw std::runtime_error("Fatal Error: Failed to load font 'assets/fonts/arial.ttf'. Make sure it's in the execution directory.");
}
```

**בדיקת רקעים אופציונליים:**
```cpp
sf::Texture bgTexture;
bool hasTexture = false;
if (bgTexture.loadFromFile("background.jpg")) {
    // שימוש ברקע תמונה
    hasTexture = true;
} else {
    // fallback לרקע צבע אחיד
    colorBackground.setFillColor(VisualStyle::PRIMARY_DARK);
}
```

### 2. היררכיית טיפול בשגיאות

#### שכבה A: בדיקות GUI רמה עליונה
```cpp
// בחירת מטרה
std::string targetName = askForTargetPlayerName();
if (targetName.empty()) {
    continue; // המשתמש ביטל הבחירה
}

// אימות קיום השחקן
bool found = false;
for (Player* p : game.getPlayers()) {
    if (p->getName() == targetName) {
        targetPlayer = p;
        found = true;
        break;
    }
}
if (!found) {
    showErrorPopup("Player '" + targetName + "' not found.");
    continue; // חזרה לבחירת פעולה
}
```

#### שכבה B: בדיקות מנוע המשחק
כל פעולת שחקן מבצעת סדרת בדיקות:

**בדיקות בסיסיות (בכל פעולה):**
```cpp
void Player::checkActive() const {
    if (!active) throw std::runtime_error("Player is not active");
}

void Player::checkTurn() const {
    if (Game::getInstance().getCurrentPlayer() != this) 
        throw std::runtime_error("wrong player's turn");
}

void Player::checkSanctioned() const {
    if (sanctioned) throw std::runtime_error("Player is sanctioned");
}

void Player::checkMustCoup() const {
    if (playerCoins >= 10) 
        throw std::runtime_error("Player must coup");
}
```

#### שכבה C: בדיקות ספציפיות לפעולה

**דוגמה - בדיקות arrest():**
```cpp
void Player::arrest(Player& other) {
    // בדיקות כלליות
    this->checkActive();
    other.checkActive();
    this->checkTurn();
    this->checkMustCoup();
    
    // בדיקות ספציפיות
    if (this->getName() == other.getName()) {
        throw std::runtime_error("Player cannot arrest themselves");
    }
    if (other.playerCoins == 0) {
        throw std::runtime_error("Player has no coins to lose");
    }
    if (other.lastArrested) {
        throw std::runtime_error("Player has already been arrested");
    }
    if (!this->canArrest) {
        throw std::runtime_error("Player cannot arrest");
    }
    
    // ביצוע הפעולה...
}
```

### 3. וולידציה מתקדמת של קלט משתמש

#### בדיקת שמות שחקנים:
```cpp
// בשלב הוספת שחקנים
if (event.text.unicode == 13 || event.text.unicode == 10) { // Enter
    if (!currentInput.empty()) {
        // בדיקת שמות כפולים
        bool nameExists = std::any_of(playerNames.begin(), playerNames.end(), 
            [&](const std::string& name) { return name == currentInput; });

        if (nameExists) {
            errorMessage = "Name already exists. Please enter a unique name.";
            errorClock.restart();
        } else if (playerNames.size() == 6) {
            errorMessage = "Maximum of 6 players reached. Please start the game.";
            errorClock.restart();
        } else {
            playerNames.push_back(currentInput);
            currentInput.clear();
            errorMessage.clear();
        }
    }
}
```

#### הגבלת תווים חוקיים:
```cpp
else if (event.text.unicode >= 32 && event.text.unicode < 128) {
    currentInput += static_cast<char>(event.text.unicode);
}
```

#### מנגנון תפוגת שגיאות:
```cpp
sf::Clock errorClock;
// ...
if (!errorMessage.empty() && errorClock.getElapsedTime().asSeconds() > 3.0f) {
    errorMessage.clear(); // הסתרת השגיאה לאחר 3 שניות
}
```

### 4. מנגנון recovery ויציאה בטוחה

#### טיפול בשגיאות advance turn:
```cpp
if (!currentPlayer->isActive()) {
    try {
        game.nextTurn();
        currentPlayer = game.getCurrentPlayer();
    } catch (const std::runtime_error& e) {
        std::cout << "Error advancing turn: " << e.what() << std::endl;
        break; // יציאה בטוחה מלולאת המשחק
    }
}
```

#### מניעת קריסת תוכנית:
```cpp
// בmain():
try {
    coup::Game& game = coup::Game::getInstance();
    coup::GameGUI gui(game);
    gui.run();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1; // יציאה עם קוד שגיאה
}
```

## אופטימיזציות ביצועים מתקדמות

### 1. ניהול זיכרון חכם

#### הקצאה מוקדמת ויעילה:
```cpp
// בGame constructor:
Game() : _players(), _currentPlayerIndex(0), _gameActive(false), _numPlayers(0) {
    _players.reserve(6); // הקצאת זיכרון מראש למקסימום שחקנים
}
```

#### שימוש ב-references במקום עותקים:
```cpp
// העברת משתנים כreference כדי למנוע העתקות מיותרות
const std::vector<Player*>& getPlayers() const { return _players; }

// גישה ישירה לשחקן נוכחי ללא העתקה
Player* getCurrentPlayer() const { return _players[_currentPlayerIndex]; }
```

#### שחרור זיכרון אוטומטי:
```cpp
~Game() {
    for (Player* player : _players) {
        delete player; // שחרור כל השחקנים
    }
    _players.clear();
}
```

#### יצירת אובייקטי גרפיקה רק כשנדרש:
```cpp
// יצירת כפתורים דינמית לפי פעולות זמינות
std::vector<ActionType> availableActions = currentPlayer->getAvailableActions();
std::vector<sf::RectangleShape> actionButtons;
std::vector<sf::Text> actionTexts;

// רק לפעולות שבאמת זמינות
for (ActionType action : availableActions) {
    if (action == ActionType::cancel) continue;
    // יצירת כפתור רק עבור פעולות רלוונטיות
}
```

### 2. רינדור מותנה ואופטימלי

#### עדכון מסך מותנה:
```cpp
// בלולאת המשחק הראשית - עדכון רק כשיש שינוי
bool needsRedraw = false;

// בדיקת שינויים במצב
if (previousPlayerIndex != game.getCurrentPlayerIndex()) {
    needsRedraw = true;
    previousPlayerIndex = game.getCurrentPlayerIndex();
}

if (needsRedraw) {
    // רינדור מלא רק כשנדרש
    window.clear();
    drawAllElements();
    window.display();
}
```

#### איחוד פעולות ציור (Batching):
```cpp
// ציור בקבוצות לפי סדר עומק
// 1. רקעים וצללים
if (hasTexture) window.draw(background);
else window.draw(colorBackground);

for (const auto& shadow : allShadows) {
    window.draw(shadow);
}

// 2. אלמנטי UI עיקריים
for (const auto& button : allButtons) {
    window.draw(button);
}

// 3. טקסט עליון
for (const auto& text : allTexts) {
    window.draw(text);
}
```

#### הגבלת קצב רינדור:
```cpp
window.setFramerateLimit(60); // 60 FPS מקסימום - חסכון במשאבי CPU
```

### 3. אירועים ואינטראקציות יעילות

#### טיפול מהיר באירועי עכבר:
```cpp
// בדיקת collision יעילה
sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
for (size_t i = 0; i < actionButtons.size(); ++i) {
    if (actionButtons[i].getGlobalBounds().contains(mousePos)) {
        // עיבוד מיידי ויציאה
        processButtonClick(i);
        break; // חשוב! מניעת בדיקות מיותרות
    }
}
```

#### מניעת lag בהקלדה:
```cpp
// עיבוד תווים מיידי ללא עיכוב
if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode == 8 && !currentInput.empty()) {
        currentInput.pop_back(); // מחיקה מיידית
    } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
        currentInput += static_cast<char>(event.text.unicode); // הוספה מיידית
    }
    inputDisplay.setString("> " + currentInput); // עדכון מיידי של התצוגה
}
```

#### זיהוי מהיר של אזורי קליק:
```cpp
// שימוש ב-getGlobalBounds() לזיהוי מהיר
// במקום חישובי מרחק מורכבים
if (button.getGlobalBounds().contains(mouseX, mouseY)) {
    // פעולה מיידית
}
```

### 4. אופטימיזציית מבני נתונים

#### שימוש ב-static maps לפעולות המרה:
```cpp
// המרות מהירות O(log n) במקום if-else ארוכים
std::string GameGUI::roleToString(Role role) const {
    static const std::map<Role, std::string> roleMap = {
        {Role::Governor, "Governor"}, {Role::Spy, "Spy"},
        // ... רק פעם אחת בזיכרון
    };
    auto it = roleMap.find(role);
    return (it != roleMap.end()) ? it->second : "Unknown";
}
```

#### גישה מהירה למידע שחקנים:
```cpp
// O(1) access לשחקן נוכחי
Player* getCurrentPlayer() const { 
    return _players[_currentPlayerIndex]; 
}

// ספירה מהירה של שחקנים פעילים
int countActivePlayers() const {
    int count = 0;
    for (const auto& player : _players) {
        if (player->isActive()) ++count;
    }
    return count;
}
```

### 5. אופטימיזציות מיוחדות לSFML

#### יצירת צורות פעם אחת:
```cpp
// פונקציות helper ליצירת אלמנטים נפוצים
sf::RectangleShape createRoundedButton(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor) {
    static sf::RectangleShape button; // יצירה פעם אחת
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(fillColor);
    // קביעת מאפיינים נוספים...
    return button;
}
```

#### שימוש יעיל בטקסטורות:
```cpp
// טעינת טקסטורות פעם אחת ושימוש חוזר
static sf::Texture backgroundTexture;
static bool textureLoaded = false;

if (!textureLoaded && backgroundTexture.loadFromFile("background.jpg")) {
    textureLoaded = true;
}
```

#### המרת פעולות (`actionTypeToString`, `stringToActionType`)
המרה דו-כיוונית בין enum ActionType ל-string עם טיפול בשגיאות:

**actionTypeToString:**
```cpp
static const std::map<ActionType, std::string> actionMap = {
    {ActionType::Gather, "Gather"}, {ActionType::Tax, "Tax"},
    {ActionType::Bribe, "Bribe"}, {ActionType::Arrest, "Arrest"},
    {ActionType::Coup, "Coup"}, {ActionType::Sanction, "Sanction"},
    {ActionType::Invest, "Invest"}, {ActionType::SpyOn, "Spy On"},
    {ActionType::cancel, "Cancel"}
};
```

**stringToActionType:**
```cpp
// המרה הפוכה עם זריקת חריגה עבור ערכים לא חוקיים
auto it = actionMap.find(actionStr);
if (it != actionMap.end()) {
    return it->second;
}
throw std::runtime_error("Invalid action string provided: " + actionStr);
```

#### בדיקת תקינות פעולות (`getAvailableActions`)
כל שחקן מחזיר רשימה דינמית של פעולות זמינות:

**בסיסי עבור כל שחקן:**
```cpp
std::vector<ActionType> actions;
actions.push_back(ActionType::Gather);
actions.push_back(ActionType::Tax);

if (playerCoins >= 4) actions.push_back(ActionType::Bribe);
if (playerCoins >= 3) actions.push_back(ActionType::Sanction);
if (playerCoins >= 7) actions.push_back(ActionType::Coup);

// בדיקה אם יש מטרות חוקיות למעצר
bool canPerformArrest = false;
for (const auto& player : Game::getInstance().getPlayers()) {
    if (player->isActive() && player != this && 
        player->getCoins() > 0 && !player->isLastArrested()) {
        canPerformArrest = true;
        break;
    }
}
if (canPerformArrest && this->canArrest) {
    actions.push_back(ActionType::Arrest);
}

return actions;
```


## מנגנון מתקדם של ביטול פעולות (Cancel Mechanism)

### רקע טכני
מערכת הביטול במשחק מבוססת על תבנית Observer עם בדיקות זמן אמת:

#### שלבי הביטול:
1. **זיהוי פעולה ראשית** - המערכת שומרת את `lastStep` ב-Game singleton
2. **סריקת שחקנים זכאים** - לולאה על כל השחקנים הפעילים
3. **בדיקת יכולת ביטול** - `canCancel(ActionType)` לכל שחקן
4. **הצגת אפשרות ביטול** - פתיחת חלון אישור למשתמש
5. **ביצוע ביטול** - קריאה ל-`cancel(Player&)` של השחקן המבטל

### אלגוריתם ביטול מפורט:
```
AFTER פעולה בוצעה בהצלחה:
    executedAction = הפעולה שבוצעה
    targetPlayer = מטרת הפעולה (אם רלוונטית)
    
    FOR כל שחקן p ברשימת השחקנים:
        IF p.isActive() AND p != השחקן הנוכחי AND p.canCancel(executedAction):
            answer = showCancelConfirmation(p.getName())
            IF answer == "yes":
                IF executedAction == ActionType::Coup:
                    p.cancel(*targetPlayer)  // General מבטל coup על המטרה
                ELSE:
                    p.cancel(*currentPlayer) // Judge/Governor מבטלים על המבצע
                BREAK  // רק שחקן אחד יכול לבטל
```

### יכולות ביטול לפי תפקיד:

#### Judge (שופט):
- **יכולת:** ביטול פעולת `Bribe`
- **עלות:** ללא עלות
- **אפקט:** `target.setIsBribed(false)`, `game.nextTurn()`

#### Governor (מושל):
- **יכולת:** ביטול פעולת `Tax`
- **עלות:** ללא עלות
- **אפקט:** הורדת מטבעות (2 רגיל, 3 למושל אחר)

#### General (גנרל):
- **יכולת:** ביטול פעולת `Coup`
- **עלות:** 5 מטבעות
- **אפקט:** `target.setActive(true)`, `game.setNumPlayers(+1)`

## מנגנון ניהול תורות מתקדם (Advanced Turn Management)

### סיבוכיות ניהול התורות
המערכת מתמודדת עם מספר אתגרים טכניים מורכבים:

#### אלגוריתם `nextTurn()` המלא:
```
FUNCTION nextTurn():
    // בדיקת תקינות מצב המשחק
    activePlayerCount = ספירת שחקנים פעילים
    IF activePlayerCount <= 1:
        THROW "Not enough players to continue"
    
    // איפוס סטטוס שחקן נוכחי
    IF currentPlayer.isBribed():
        currentPlayer.setIsBribed(false)
    
    // בונוס מיוחד למרצ'נט
    IF currentPlayer.role == Merchant AND currentPlayer.coins >= 3:
        currentPlayer.coins += 1
        PRINT "Merchant bonus coin"
    
    // מעבר לשחקן הבא הפעיל
    DO:
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size()
    WHILE NOT players[currentPlayerIndex].isActive()
    
    CALL turn()  // הודעה על תור חדש
```

### מצבי מעבר תור מורכבים:

#### פעולת Bribe:
- **לוגיקה:** הפעולה אינה מקדמת תור
- **מימוש:** בדיקת `lastStep != Bribe` לפני `nextTurn()`
- **מגבלה:** שחקן יכול לבצע שתי פעולות נוספת 
#### פעולת Coup עם שחקן יחיד:
- **לוגיקה:** בדיקת `numPlayers > 1` לפני `nextTurn()`
- **מימוש:** מניעת קריאה ל-`nextTurn()` בסיום המשחק

#### דילוג על שחקנים לא פעילים:
- **אלגוריתם:** לולאת `do-while` עם בדיקת `isActive()`
- **מימוש:** מודולו על גודל המערך למעבר מעגלי

## אינטגרציה מלאה בין GUI למנוע המשחק

### דפוס Singleton המשולש
```
GameGUI ←→ Game ←→ Player
  ↑                    ↑
  └─── SFML Events ────┘
```

#### זרימת נתונים דו-כיוונית:

**מ-GUI למנוע:**
- בחירת פעולה → `stringToActionType()`
- בחירת מטרה → `askForTargetPlayerName()`
- ביצוע פעולה → `currentPlayer->action()`

**מהמנוע ל-GUI:**
- מצב שחקן → `getCurrentPlayer()`, `getCoins()`, `isActive()`
- רשימת פעולות → `getAvailableActions()`
- סטטוס משחק → `getNumPlayers()`, `getLastStep()`

### מנגנון בדיקת שגיאות מרובד:

#### שכבה 1: בדיקות GUI
```cpp
IF targetName.empty():
    continue  // המשתמש ביטל
IF !found player with targetName:
    showErrorPopup("Player not found")
    continue
```

#### שכבה 2: בדיקות מנוע
```cpp
// בתוך כל פעולת שחקן:
checkActive()     // שחקן פעיל?
checkTurn()       // התור שלו?
checkSanctioned() // לא תחת סנקציה?
checkMustCoup()   // חייב coup (10+ מטבעות)?
```

#### שכבה 3: בדיקות ספציפיות
```cpp
// לדוגמה ב-arrest():
IF target.coins == 0:
    THROW "Player has no coins to lose"
IF target.lastArrested:
    THROW "Player has already been arrested"
IF !this.canArrest:
    THROW "Player cannot arrest"
```

## מכניקות מיוחדות לכל תפקיד (Role-Specific Mechanics)

### Governor (מושל) - מימוש מפורט:
```cpp
// Override של tax()
void Governor::tax() {
    checkActive();
    checkSanctioned(); 
    checkTurn();
    checkMustCoup();
    playerCoins += 3;  // במקום 2 רגיל
    if (lastStep != Bribe) {
        nextTurn();
        canArrest = true;
    }
    setLastStep(ActionType::Tax);
}

// יכולת ביטול מיוחדת
void Governor::cancel(Player& target) {
    IF lastStep != Tax: THROW "governor can cancel only tax"
    IF target == this: THROW "cannot undo their own Action"
    
    // החזרת מטבעות לפי תפקיד המטרה
    IF target.role == Governor:
        target.coins -= 3
    ELSE:
        target.coins -= 2
    
    setLastStep(ActionType::cancel)
}
```

### Merchant (סוחר) - בונוס אוטומטי:
```cpp
// בתוך nextTurn():
IF currentPlayer.role == Merchant AND currentPlayer.coins >= 3:
    currentPlayer.coins += 1
    PRINT "Merchant received bonus coin"
```

### General (גנרל) - הגנה פסיבית ופעילה:
```cpp
// הגנה פסיבית ב-arrest():
ELSE IF other.role == General:
    this.playerCoins += 1  // תוקף מקבל מטבע נוסף
    // General לא מאבד מטבע!

// ביטול coup (5 מטבעות):
void General::cancel(Player& target) {
    IF lastStep != Coup: THROW "General can cancel only coup"
    IF target == this: THROW "cannot undo their own Action"
    IF coins < 5: THROW "Not enough coins to prevent coup"
    
    coins -= 5
    game.setNumPlayers(game.getNumPlayers() + 1)
    target.setActive(true)  // החייאת השחקן
    setLastStep(ActionType::cancel)
}
```

### Spy (מרגל) - מידע ומניעה:
```cpp
void Spy::spyOn(Player& target) {
    // ... בדיקות רגילות ...
    // בGUI: viewPlayerCoinsPopup(target.getName())
    setLastStep(ActionType::SpyOn)
}

// מניעת arrest (ב-canCancel):
bool Spy::canCancel(ActionType action) const {
    return action == ActionType::Arrest;
}
```

## מנגנון ניהול מצב מתקדם (Advanced State Management)

### איפוס אוטומטי של דגלים:
כל פעולה מבצעת איפוס של מצבים זמניים:

```cpp
// בסוף כל פעולה (אם לא Bribe):
IF lastStep != Bribe:
    nextTurn()
    canArrest = true      // איפוס יכולת מעצר
    sanctioned = false    // איפוס סנקציה
    isBribed = false      // איפוס במנגנון nextTurn
```

### ניהול מצב `lastArrested`:
```cpp
// בתחילת כל arrest():
FOR player IN allPlayers:
    player.lastArrested = false  // איפוס כללי

// בסוף arrest():
target.lastArrested = true  // סימון המטרה החדשה
```

### מצבי סיום מורכבים:
```cpp
// ב-runGameScreen():
activePlayerCount = ספירה בפועל של שחקנים פעילים

IF activePlayerCount <= 1:
    IF activePlayerCount == 1:
        // מציאת הניצחון האחרון
        FOR player IN players:
            IF player.isActive():
                showWinnerScreen(player.getName())
                RETURN
    showWinnerScreen("No one")  // אף אחד לא נותר
    RETURN
```

## אופטימיזציות ביצועים ותכנותיות

### בדיקות מותנות חכמות:
```cpp
// בלולאת המשחק הראשית:
IF !currentPlayer.isActive():
    // ספירה מהירה של שחקנים פעילים
    IF activePlayerCount <= 1:
        // סיום מיידי
    ELSE:
        // מעבר בטוח לשחקן הבא
```

### שימוש במיקום זיכרון יעיל:
```cpp
// Game constructor:
_players.reserve(6);  // הקצאה מראש

// בגישה לשחקן נוכחי:
Player* getCurrentPlayer() const { 
    return _players[_currentPlayerIndex]; 
}  // גישה O(1)
```

### מניעת זליגות זיכרון:
```cpp
// Game destructor:
~Game() {
    for (Player* player : _players) {
        delete player;  // שחרור כל השחקנים
    }
    _players.clear();
}
```

## מבנה אירועים ואינטראקציות מתקדם

### Event Loop מותאם למשחק:
```cpp
WHILE window.isOpen() AND !turnEnded:
    WHILE pollEvent():
        IF event.type == MouseButtonPressed:
            FOR button IN actionButtons:
                IF button.contains(mousePos):
                    action = stringToActionType(button.text)
                    TRY:
                        executeAction(action)
                        turnEnded = true
                        
                        // בדיקת אפשרויות ביטול
                        checkCancellationOpportunities()
                    CATCH error:
                        showErrorPopup(error.message)
                        // תור ממשיך, אין סיום
```

### ניהול חלונות מודליים:
```cpp
// בכל חלון עזר:
WHILE popupWindow.isOpen():
    processEvents()
    
    // תמיד עם אפשרות יציאה:
    IF Escape OR Close:
        popupWindow.close()
        RETURN defaultValue
```

## סיכום מקיף

מערכת ה-GUI של משחק Coup מייצגת מימוש מתוחכם של ממשק גרפי אינטראקטיבי המשלב:

### נקודות חוזק עיקריות:
1. **ארכיטקטורה מודולרית** - הפרדה ברורה בין UI למנוע
2. **טיפול מקיף בשגיאות** - מספר שכבות בדיקה ואימות
3. **מנגנון ביטול מתקדם** - תמיכה בכל סוגי הביטולים
4. **ניהול מצב מורכב** - שמירה על קונסיסטנטיות המשחק
5. **חוויית משתמש מעולה** - עיצוב מודרני ואינטואיטיבי

### הלוגיקה המרכזית:
המערכת פועלת כ-State Machine מורכב עם מעברי מצב מותנים, תוך שימוש בדפוס Singleton לניהול מצב גלובלי ותמיכה מלאה בכללי המשחק המורכבים. האינטגרציה בין הרכיבים השונים מושגת באמצעות API מוגדר היטב עם טיפול מקיף בחריגות ושגיאות.

הממשק הגרפי לא רק מציג את מצב המשחק, אלא משתתף באופן פעיל בניהול הלוגיקה, תוך הבטחת שהמשתמש יקבל חוויה עקבית ומהנה ללא קשר למצב המשחק או לתפקיד השחקן.
