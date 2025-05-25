# project-ex2
**Part 2 of Gmail project**  
**Members: Shaked Shlasky Eliran Vasserman Dvir Avidan**  
  
Dear Tutor, we hope you have a pleasant testing, here are table of contents in readme.md:  
  
1. How to run the application and execute the tests  
2. Directory structure and file organization (including code, data, and other relevant components)  
3. Application architecture: main objects and interfaces  
4. General issues or limitations  
5. Lecturer Guidelines and Design Reflections  
6. Running Examples (From Docker)  
  
Github repository link: https://github.com/Bar-Ilan-Bros/project-ex2  
Jira project link: https://eliranvass7425.atlassian.net/jira/software/projects/PE/boards/34  

    
```
    .--.                       ##        .               .--.  
   |o_o |                ## ## ##       ==              |o_o |  
   |:_/ |             ## ## ## ##      ===              |:_/ |  
  //   \ \        /""""""""""""""""\___/ ===           //   \ \  
 (|     | )  ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~   (|     | )  
/'\_   _/`\       \______ o          __/             /'\_   _/`\  
\___)=(___/        \    \        __/                 \___)=(___/   
                    \____\______/                                 
                                                                      
                      |          |                                   
                   __ |  __   __ | _  __   _                         
                  /  \| /  \ /   |/  / _\ |                          
                  \__/| \__/ \__ |\_ \__  |                            
```  
  
-------------------------------------------------------------------------------------------------------------  
**1. HOW TO RUN OUR APPLICATION AND TESTS**  
-------------------------------------------------------------------------------------------------------------  
There is a *Single Docker Image* which including the whole files and data needed (for both app and tests). We have implemented the Docker Container user interface as an interactive bash terminal.  
We will use *Multiple Containers* to demonstrate the server&client architecture.  
  
A) Run the docker-compose file:  
**docker-compose up --build -d**  
(From main directory of project)  
(Creates image and shared network. Then 2 containers named: server_container, client_container)  
  
B) Enter server container:  
**docker exec -it server_container bash**  
  
C) Open another bash terminal.  
  
D) Get server container's ip (In use for 'G' instruction):  
**docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' server_container**  
(Works from every environment: Windows, Linux, etc)  
  
E) Enter client container:  
**docker exec -it client_container bash**  
  
F) Run the server in the "server_container" terminal:  
**./main [Port] [Bloom_Setup_Line]**  
(Port is mandatory, Bloom_Setup_Line is mandatory for first run only - next runs will be ignored)  
  
G) Run the client in the "client_container" terminal:  
**python3 Client.py [IP] [Port]**  
Argument of "IP" in Client.py is the Server Container's IP (from part E).  
Argument of "Port" in Client.py is the same "Port" as in Server (from part F).
  
H) Running tests (Inside the container):  
**./[file_name]**  
'ids_tests', 'io_tests', 'command_tests', 'setup_tests', 'menu_tests', 'app_tests'  
  
(Important - Database saved inside docker container)  
(To stop server infinite loop you can click CTRL+C)  
(Notice that for our check you should use our regex explained in README part 4 - General Issues)  
  
I) Exit Docker Container: exit (CTRL+D as well)  
  
  
**Extra**  
Check the Application's database inside the container's terminal:  
cd ../../data  
cat url.txt  
cat setup.txt  
  
Use the next command to return to the executable directory:  
cd ../src/exe  
  
  
  
**Manual Commands Option (Not Recommended - without docker compose):**  
*If there is any issue here is option to run by single commands:*  
A) Build the image:  
*docker build -t proj_ex2 .*  
B) Create shared network:    
*docker network create my_net*  
C) Create server container:  
*docker run -it --name server_container --network my_net proj_ex2*  
D) Enter server container:  
*docker exec -it server_container bash*  
E) Open another bash terminal.  
F) Get server container's ip:     
*docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' server_container*  
G) Create client container:  
*docker run -it --name client_container --network my_net proj_ex2*  
H) Enter client container:  
*docker exec -it client_container bash*  
I) Run in the server's terminal:  
*./main [Port] [Bloom_Setup_Line]*  
J) Run in the client's terminal:  
*python3 Client.py [IP] [Port]*  
Argument of "IP" in Client.py is the Server Container's IP (from part E).  
Argument of "Port" is the same between Server and Client.
  
K) Running tests (Choose a container and run wanted test file):  
./[file_name]  
'ids_tests', 'io_tests', 'command_tests', 'setup_tests', 'menu_tests', 'app_tests'  
  
  
  
-------------------------------------------------------------------------------------------------------------  
**2. DIRECTORY STRUCTURE AND FILE ORGANIZATION**  
-------------------------------------------------------------------------------------------------------------  
```
project-ex2   
├── Dockerfile                         Docker environment image creation file  
├── README.md                          Project instructions and documentation  
├── data                               Directory to store the data inside: urls, setup configuration  
│   └── data.txt                       Dummy file to keep the directory in GitHub  
├── details.txt                        Team members' details (submitted to Lemida system)  
├── docker-compose.yml                 Docker compose file to configure multi-container application  
├── proofs                             Directory to document progress in the project  
└── src  
    ├── AddCommand.cpp                 Implements the command of add URL to BloomFilter using command interface  
    ├── AddCommand.h                   Declares object  
    ├── App.cpp                        Initializes and runs the main application infinite loop.  
    ├── App.h                          Declares object  
    ├── AppTest.cpp                    Tests for the App and Main epic.  
    ├── BloomFilter.cpp                Implements the core Bloom filter logic, including Adding or Checking URLs  
    ├── BloomFilter.h                  Declares object
    ├── CMakeLists.txt                 Generates Make commands for building executables  
    ├── CheckCommand.cpp               Implements command of check URL to BloomFilter using command interface  
    ├── CheckCommand.h                 Declares object  
    ├── Client.py                      Python Client works with the server  
    ├── CommandTest.cpp                Tests for the Command epic.  
    ├── ConsoleMenu.cpp                Implements the Command Design Pattern using menu constants.  
    ├── ConsoleMenu.h                  Declares object  
    ├── DataStructureTest.cpp          Tests for the DataStructure epic.  
    ├── DeleteCommand.cpp              Implements the Command of delete a URL from Bloom (Command Interface)  
    ├── DeleteCommand.h                Declares object  
    ├── HashFunctions.cpp              Implements hash functions used by the Bloom filter to map URLs.  
    ├── HashFunctions.h                Declares object  
    ├── HashFunctionsTestManual.cpp    Manual tests for hash functions not in use (debugging gtest at start)  
    ├── ICommand.h                     Declares the interface of Commands  
    ├── IDataStructure.h               Declares the interface of DataStructure  
    ├── IIO.h                          Declares the interface for input/output operations  
    ├── IMenu.h                        Declares the interface for the menu logic  
    ├── IOTest.cpp                     Tests for input/output functionalities in the project  
    ├── ISetup.h                       Declares the interface for setting up the Bloom while the app launches  
    ├── LineSetup.cpp                  Implements the logic for setting up the Bloom at launch for ex-1  
    ├── LineSetup.h                    Declares object  
    ├── Main.cpp                       Contains the entry point to the application and the configurations to app  
    ├── MenuTest.cpp                   Tests for the Menu epic  
    ├── SetupTest.cpp                  Tests for the Setup epic  
    ├── SocketIO.cpp                   Implements socket-based input/output operations  
    ├── SocketIO.h                     Declares object for socket-based input/output operations  
    ├── StreamIO.cpp                   Implements stream-based input/output operations  
    ├── StreamIO.h                     Declares object for stream-based input/output operations  
    └── exe                            Directory to store the main&tests executables (within container only)
        └── Client.py                  Python Client works with the server  
```
  
  
  
-------------------------------------------------------------------------------------------------------------  
**3. APPLICATION ARCHITECTURE**  
-------------------------------------------------------------------------------------------------------------  
  
**A. IDataStructure (Interface)**  
Related Objects:  
BloomFilter (implements IDataStructure)  
HashFunctions (member of BloomFilter)  
  
**B. ICommand (Interface)**  
Related Objects:  
AddCommand (implements ICommand)  
CheckCommand (implements ICommand)  
DeleteCommand (impelements ICommand)  
  
**C. IMenu (Interface)**  
Related Objects:  
ConsoleMenu (implements IMenu)  
  
**D. ISetup (Interface)**  
Related Objects:  
LineSetup (implements ISetup)  
  
**E. IIO (Interface)**  
Related Objects:  
StreamIO (implements IIO)  
SocketIO (implements IIO)  
  
**F. App (Object)**  
Members:  
IDataStructure (BloomFilter object only at the moment)  
IMenu (ConsoleMenu object only at the moment)  
SocketIO (Two fields: input, output. At the moment usage is with same instance)  
String (Two fields: url_path, setup_path. Saving our data locations)  
  
  
  
-------------------------------------------------------------------------------------------------------------  
**4. GENERAL ISSUES**  
-------------------------------------------------------------------------------------------------------------  
A) URL format which the application accepts is according to next regex:  
  
R"(^\s*((https?|HTTPS?)://)?([a-zA-Z0-9-]+.)+[a-zA-Z]{2,}([a-zA-Z0-9-./%&=])?\s$)" Generally: you are allowed start with https:// or http:// or not starting pattern. Whitespace at middle of URL is NOT allowed. Whitespace at start or end of URL in allowed (and needed) by comment on lambda's forum by lecturer (but note it would be stored and checked ignoring these whitespaces). The rest is standard by google: www.allowed.example.com, www.num123.com, www.fill.com/free-to-use-123-end  
  
B) Case Sensitiveness: As known in real life URL could be both UPPER CASE and LOWER CASE. We will use real life rule. First we define 'Domain' and 'Path' with an example: "www.example.com/path/of/url" DOMAIN = "www.example.com" is RESPECTING different cases. PATH = "path/of/url" is NOT RESPECTING different cases. Thus, our program will return "true" value for different lower/upper case input url in the 'domain' part, and "false" if it was in the 'path' part.  
For consistency, our database is saving all the url's domain part in LOWERCASE.  

C) Running Main (Server) Arguments *[Port] [Bloom_Setup_Line]*:  
Port is mandatory for every run of the server.  
Bloom_Setup_Line is mandatory only for the first run of the server.  
Next launches of the server will ignore the Bloom_Setup_Line arguments - The bloom uses the first run arguments.  
  
D) Docker container is saving the data between *different runs* of the container.  
Notice to use the same exact container using our commands and not creating a new one or deleting it by a mistake.  
  
E) Data Files Check - If you would like to check the data files which store url and configuration - you may use the docker interactive bash to change dir into data directory, for your convinience:  
  
Change to data directory & peek the file using these commands:  
*cd ../../data*  
*cat url.txt*  
*cat setup.txt*  
  
Back to executable directory command:  
*cd ../src/exe*  
  
  
  
-------------------------------------------------------------------------------------------------------------  
**5. Lecturer Guidelines and Design Reflections**  
-------------------------------------------------------------------------------------------------------------  
This section represents Hemi's points to reflect in the readme - page 3 of task pdf file.  
  
A) *Did the fact that command names changed require you to modify code that is supposed to be "closed for modification but open for extension"?*  
**No!** We defined well a Menu object in part 1 of the project. Thus, we had to change only the ConsoleMenu class to handle another "legal" command notations. In addition, we sent smartly the remainder of the command (URL itself) into a buffer. This method made the rest of the code correct while no need to modify.  
  
B) *Did the addition of new commands require you to modify code that is supposed to be "closed for modification but open for extension"?*  
**No!** We defined well a "Command Design Pattern" in part 1. Thus, we had to add a single Class named "DeleteClass", which the ICommand polymorphism let us not changing the Applications Code or Bloom Filter Code at all.  
  
C) *Did changes in the command output require you to modify code that is supposed to be "closed for modification but open for extension"?*  
**Yes**, but minor. We had to change output on each Command Object (Add/Check) from part 1, because we had a print command inside each command. Which made us to change each Class of Command code, that should be closed for modification. It is generally fine, because modification is part of command.  
But! *We improved the code using mapping of outputs!* Each single case of all command get mapped into an integer. This integer is mapped into an output using main (Outside the Commands). Thus, for next parts we will be able to change our outputs freely, without modification of "Command Classes". The print responsibility moved to main function.  
  
D) *Did the fact that input/output now comes from sockets instead of the console require you to modify code that is supposed to be "closed for modification but open for extension"?*  
**No**, we didn't have to "correct our part 1's code". **But**, we had to implement a new Object and new IO interface for socket input/output and for stream(file/console) input/output.  
The previous part, we used c++ gifts while using stream/fstream/istream/ostream relations (polymorphism), in assumption that future modification will be into a file (instead of console). Unfortunately, we had to io into a socket. That we implemented our own input/output interface which contains Stream Class and Socket Class.  
Using this method we recieved and send io from clients.  
  
  
-------------------------------------------------------------------------------------------------------------  
**6.RUNNING EXAMPLES**  
-------------------------------------------------------------------------------------------------------------  
*A) Run from clear database:*  
(Server Side)  
./main 8020 8 1 2  
  
(Client Side)  
python3 Client.py 172.19.0.2 8020  
POST www.example.com  
201 Created  
GET www.example.com  
200 Ok  
  
true true  
  
  
*B) Close and reopen the program:*  
(No bloom settings arguments needed, Database should be saved)  
(Server Side)  
./main 8020  
  
(Client Side)  
python3 Client.py 172.19.0.2 8020  
GET www.example.com2  
200 Ok  
  
false  
GET www.example.com  
200 Ok  
  
true true  
  
  
*C) Check illegal inputs (Illegal command, illegal url, illegal syntax):*  
(Client Side)  
asdsa  
400 Bad Request  
POST 12321      
400 Bad Request  
POST www.example.com www.example2.com  
400 Bad Request  
  
  
*D) Check logical problem inputs (Delete URL no exists):*  
(Client Side)  
DELETE www.dijkstra.com  
404 Not Found  
  
  
*E) Check lowercase/highercase synchronization:*  
(Respecting cases in domain, not respecting cases in path)  
(Client Side)  
POST https://www.domain/path          
201 Created  
GET https://www.DOMAIN/path  
200 Ok  
  
true true  
GET https://www.domain/PATH  
200 Ok  
  
false  
  
  
*F) Adding & Checking then Deleting & Checking:*  
(Should be false-positive because bits are saved)  
(Client Side)  
POST www.fft.com  
201 Created  
GET www.fft.com  
200 Ok  
  
true true  
DELETE www.fft.com  
204 No Content  
GET www.fft.com  
200 Ok  
  
true false  
  
  
*G) Running client&quit, then launch another client:*  
(Should continue working for new clients)  
  
(Client1)  
python3 Client.py 172.19.0.2 8020  
POST dijkstra.king.com  
201 Created  
GET dijkstra.king.com      
200 Ok  
  
true true  
GET belman.ford.loser  
200 Ok  
  
false  
  
(Client2)  
python3 Client.py 172.19.0.2 8020  
GET dijkstra.king.com  
200 Ok  
  
true true  
GET p.equal.np.com  
200 Ok  
  
false  
DELETE dijkstra.king.com  
204 No Content  
  
(Client3)  
python3 Client.py 172.19.0.2 8020  
GET dijkstra.king.com  
200 Ok  
  
true false  
DELETE p.equal.np.com  
204 No Content  
  
  
-------------------------------------------------------------------------------------------------------------  
**THE-END**  
-------------------------------------------------------------------------------------------------------------  