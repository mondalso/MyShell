#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Function declarations for built-in shell commands
int builtin_cd(vector<string>& args);
int builtin_help(vector<string>& args);
int builtin_exit(vector<string>& args);

// List of built-in commands, followed by their corresponding functions
vector<string> builtin_str = {
    "cd",
    "help",
    "exit"
};

// Corresponding function pointers for built-in commands
vector<int(*)(vector<string>&)> builtin_func = {
    &builtin_cd,
    &builtin_help,
    &builtin_exit
};

// Return the number of built-in commands
int num_builtins() {
    return builtin_str.size();
}

// Implementation of the 'cd' command
int builtin_cd(vector<string>& args) {
    if (args.size() < 2) {  // Check if an argument is provided
        cerr << "lsh: expected argument to \"cd\"\n";
    } else {
        // Attempt to change directory
        if (chdir(args[1].c_str()) != 0) {
            cerr << "error: cd failed\n";  // Print error if 'cd' fails
        }
    }
    return 1;
}

// Implementation of the 'help' command
int builtin_help(vector<string>& args) {
    cout << "Type program names and arguments, and hit enter.\n";
    return 1;
}

// Implementation of the 'exit' command
int builtin_exit(vector<string>& args) {
    return 0;  // Returning 0 will exit the shell loop
}

// Read a line of input from the user
string readLine(){
    string input;

    if(!getline(cin, input)){  // Get a line from standard input
        if(cin.eof()){  // Exit if EOF is encountered
            exit(EXIT_SUCCESS);
        }else{
            cerr << "Error reading line" << endl;
            exit(EXIT_FAILURE);
        }
    }
    return input;
}

// Split a line into tokens, handling quotes
vector<string> splitLine(const string& line) {
    vector<string> tokens;
    string token;
    istringstream stream(line);
    char quote = 0;
    bool in_quote = false;

    while (stream) {
        char ch = stream.get();

        if (ch == '\'' || ch == '"') {  // Handle quotes
            if (!in_quote) {
                in_quote = true;
                quote = ch;
            } else if (quote == ch) {
                in_quote = false;
                tokens.push_back(token);  // End of quote, add token
                token.clear();
            } else {
                token += ch;  // Add quote character to token
            }
        } else if (isspace(ch) && !in_quote) {  // Handle spaces outside of quotes
            if (!token.empty()) {
                tokens.push_back(token);  // Add token to list
                token.clear();
            }
        } else if (ch != EOF) {
            token += ch;  // Add character to token
        }
    }

    if (in_quote) {  // Handle mismatched quotes
        cerr << "error: mismatched quotes" << endl;
        tokens.clear();
    } else if (!token.empty()) {
        tokens.push_back(token);  // Add last token if it exists
    }

    return tokens;
}

// Launch a process for non-built-in commands
int launchProcess(const vector<string>& args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        vector<char*> c_args;
        c_args.reserve(args.size() + 1);  // Reserve space for arguments

        // Convert vector<string> to vector<char*>
        for (const auto& arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(nullptr);  // Null-terminate the array

        // Execute the command
        if (execvp(c_args[0], c_args.data()) == -1) {
            cerr << "Error executing command: " << args[0] << endl;
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Error forking
        cerr << "Fork failed" << endl;
    } else {
        // Parent process waits for the child to finish
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        // Check exit status and print an error message if non-zero
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            cerr << "error: command exited with code " << WEXITSTATUS(status) << endl;
        }
    }

    return 1;
}

// Execute shell built-in command or launch a program
int execute(vector<string>& args) {
    if (args.empty()) {
        return 1;  // Empty command, continue shell loop
    }

    // Check if the command is a built-in command
    for (int i = 0; i < num_builtins(); i++) {
        if (args[0] == builtin_str[i]) {
            return (*builtin_func[i])(args);
        }
    }

    // If not built-in, launch the process
    return launchProcess(args);
}

// Main shell loop
void loop(){
    string line;
    vector<string> args;
    int status;

    do {
        cout << "\n";  // Newline for better formatting
        cout << "$ ";  // Shell prompt
        line = readLine();  // Read input from the user
        args = splitLine(line);  // Split the line into arguments
        status = execute(args);  // Execute the command
    } while(status);  // Repeat until status is 0 (exit command)
}

// Main function
int main() {
    loop();  // Start the shell loop
    return EXIT_SUCCESS;
}
