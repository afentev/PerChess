#include <map>

//#include "thc.h"
//#include "CompTest.h"
//#include "IterativeTablesEngine.h"
#include "UI/interface.h"

int main(int argc, char *argv[]) {
//    p();
//    go();

//    play();

    QApplication a(argc, argv);
    auto* window = new Ui_MainWindow;
    window->setupUi();

    a.installEventFilter(window);
    window->show();
    QApplication::exec();
    return 0;
}

//int main() {
//    std::set<std::string> used;
//    int i = 0;
//    std::map<std::string, std::map<std::string, int>> d;
//    bool cr = false;
//    for (const auto & file : std::filesystem::directory_iterator("/Users/user/Downloads/Lichess Elite
//    Database")) {
//        std::cout << file.path() << std::endl;
//        std::ifstream f;
//        f.open(file.path());
//        if (f.is_open()) {
//            std::string line;
//            std::string s;
//            while (getline (f,line)) {
//                if (line[0] == '[') {
//                    s = "";
//                    continue;
//                } else if (!line.empty()) {
//                    s += line + ' ';
//                } else if (!s.empty()) {
//                    std::string buf;
//                    std::stringstream ss(s);
//
//                    thc::ChessRules rules;
//                    thc::Move move = thc::Move();
//                    int w = 0;
//                    while (ss >> buf) {
//                        if (!std::isdigit(buf[0])) {
//                            if (w >= 19) {
//                                break;
//                            }
//                            std::string q = rules.ForsythPublish();
//                            d[q][buf]++;
//                            move.NaturalIn(&rules, buf.c_str());
//                            rules.PlayMove(move);
//                            w++;
//                        }
//                    }
//                }
//            }
//            f.close();
//        }
//    }
//
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//    char *sql;
//
//    /* Open database */
//    rc = sqlite3_open("test1.db", &db);
//
//    if( rc ) {
//        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//        return(0);
//    } else {
//        fprintf(stdout, "Opened database successfully\n");
//    }
//
//    for (const auto& pair: d) {
//        try {
//            std::pair<std::string, int> maxima1;
//            std::pair<std::string, int> maxima2;
//            maxima1.second = -1;
//            maxima2.second = -1;
//            for (auto moves: pair.second) {
//                if (moves.second > maxima1.second) {
//                    maxima2 = maxima1;
//                    maxima1 = moves;
//                } else if (moves.second > maxima2.second) {
//                    maxima2 = moves;
//                }
//            }
//            std::string m1 = (maxima1.second >= 20? std::string("'") + maxima1.first + std::string("'"): "NULL");
//            std::string m2 = (maxima2.second >= 20? std::string("'") + maxima2.first + std::string("'"): "NULL");
//            if (maxima1.second >= 20) {
//                auto s = std::string("INSERT INTO MOVES (ID,MOVE1,MOVE2) ") +
//                         std::string("VALUES ('") + pair.first +
//                         std::string("', ") + m1 +
//                         std::string(", " + m2 + ");");
//                rc = sqlite3_exec(db, s.c_str(), callback, nullptr, &zErrMsg);
//
//                if( rc != SQLITE_OK ){
//                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//                    sqlite3_free(zErrMsg);
//                } else {
//                    fprintf(stdout, "Records created successfully\n");
//                }
//
//            }
//        } catch (...) {
//            std::cerr << "Error!" << std::endl;
//            continue;
//        }
//    }
//    sqlite3_close(db);
//    return 0;
//}
//
// 5.47 gb