// Structure to track the state of a button (whether it is pressed and if its state has changed)
struct Button_State
{
    bool is_down; // Indicates whether the button is currently pressed
    bool changed; // Tracks if the button state has changed since the last frame
};

// Enumeration to define button indices for easy access
enum
{
    BUTTON_UP,    // Represents the "Up" button
    BUTTON_DOWN,  // Represents the "Down" button
    BUTTON_LEFT,  // Represents the "Left" button
    BUTTON_RIGHT,
    BUTTON_ENTER,// Represents the "Right" button
    BUTTON_W,     // Represents the "W" button
    BUTTON_S,     // Represents the "S" button
    BUTTON_COUNT  // Ensures this is always the last entry to specify the total button count
};

// Structure to store the current input state, including all button states
struct Input
{
    Button_State buttons[BUTTON_COUNT]; // Array to store the state of each button
};