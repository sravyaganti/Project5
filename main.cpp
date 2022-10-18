//Project UID db1f506d06d84ab787baf250c265e24e
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include "csvstream.h"
#include <cmath>

using std::string;
using std::log;
using std::set;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::istringstream;

class ReadCsv {
    public:
    //function that creates the big tree and calls other function to add the mini trees to it
    
    void growMommyTree(){
        csvstream csvin(fileName);
        map<string, string> row;
        while (csvin >> row) {
            for (auto &col:row) {
                string datum = col.second;
                string column_name = col.first;
                if(column_name=="tag" && mommyMap->count(datum)==0) {
                    std::map<std::string, std::map<std::string, int>>::iterator it = mommyMap->begin();
                    std::map<std::string, int> baby = growBabyTree(datum); 
                    mommyMap->insert(it, std::pair<string, map<string, int>>(datum, baby)); 
                }
            }
        }  
    }

    //function that constructs and returns a mini tree
    std::map<std::string, int> growBabyTree(string s) {
        std::map<std::string, int> * babyMap = new std::map<std::string, int>; 
        
        csvstream csvin(fileName);
        vector<std::string> posts;
        map<string, string> row;
        
        while (csvin >> row) {
            if(row["tag"]==s){
                posts.push_back(row["content"]);
            }
        }
       
        vector<set<std::string>> postWords;
      
        for(size_t i=0; i<posts.size(); i++){
            set<string> temp;
            istringstream ss(posts[i]);
            std::string word="";
            while(ss>>word){
                temp.insert(word); 
            }
            postWords.push_back(temp);
        }
        

    //outer vector in postWords contains each post, inner vector contains unique words in each post, 
    //insert values into baby map      
        for(int i = 0; i < postWords.size(); ++i) {
            for(int j = 0; j < postWords[i].size(); ++j) {
                set <std::string> currentPost = postWords[i]; 
                auto it = currentPost.begin();
                advance(it, j);
                string currentWord = *it; 
                if(babyMap->count(currentWord) == 0) {
                    std::map<std::string, int>::iterator it = babyMap->begin();
                    babyMap->insert(it, std::pair<string,int>(currentWord,1)); 
                } else if( babyMap->count(currentWord)==1){
                    auto currentIndex = babyMap->find(currentWord); 
                    int & tally = currentIndex->second;
                    ++tally;            
                }
            }
        }
        return *babyMap; 
    }

    //function that counts how many unique words there are, and 
    //how many posts per label and updates private variables
    void counter() {
        csvstream csvin(fileName);
        vector<std::string> allWords;
        vector<std::string> allPosts;
        map<string, string> row;
        numPosts=0;
        while (csvin >> row) {
            numPosts++;
            allPosts.push_back(row["content"]);
            if(postStats->count(row["tag"])==0){
                std::map<string,int>::iterator it = postStats->begin();
                postStats->insert(it, std::pair<string,int>(row["tag"], 1));
            }
            else{
                (*postStats)[row["tag"]] += 1;
            }
        }
        std::string word="";
        for(size_t i=0; i<allPosts.size(); i++){
            istringstream source (allPosts[i]);
            while(source>>word){
                uniqueWords.insert(word);
            }
        }
        vocabSize=uniqueWords.size();

    }
    //function that prints out all the training data for final output 
    void trainingPrinter() {
        cout<<"training data:"<<endl;
        csvstream csvin(fileName);
        map<string, string> row;
        while (csvin >> row) {
            cout<<"  label = ";
            cout<<row["tag"]<<", content = "<<row["content"]<<endl;    
        }
    }
    //function that prints out vocabulary size
    void printVocabSize() {
        cout<<"vocabulary size: "<<vocabSize<<endl; 
        cout<<endl;
    }
    //getter functions
    int getVocabSize() {
        return vocabSize; 
    }

    int getNumPosts() {
        return numPosts;
    }

    std::map<std::string, int> * get_postStats(){
        return postStats;
    }

    std::set<string> get_uniqueWordsSet(){
        return uniqueWords;
    }

    std::map<std::string, std::map<std::string, int>> * get_mommyMap(){
        return mommyMap;
    }
    //constructor
    ReadCsv(std::string fileName_in):
    fileName(fileName_in){
        vocabSize=0;
        numPosts=0;
        mommyMap = new std::map<std::string, std::map<std::string, int>>;
        postStats = new std::map<std::string, int>;
    }
    //destructor
    ~ReadCsv() {
        delete mommyMap;
        delete postStats; 
    } 
    private:
        //variable that keeps track of how many unique words there are (vocabulary size)
        int vocabSize;
        //variable that counts how many posts there are
        int numPosts;
        //map of maps 
        std::map<std::string, std::map<std::string, int>> * mommyMap;
        //fileName variable
        std::string fileName;
        //map that keeps track of how many posts per label
        std::map<std::string, int> * postStats;
        //set of unique words
        std::set<string> uniqueWords;
};




class Classifier {
    public:
    // traverse set
    std::string traverseSet(std::set<std::string> set, int i) {
        auto it = set.begin();
        advance(it, i);
        return *it; 
    }

    //function that alphabetically prints out classifier parameters
    void printClassifierParameters(){
        cout<<"classifier parameters:"<<endl;
        // int totalWordsUnderLabel = 0;
        for(size_t i=0; i<labels.size(); i++){
            
            string currentLabel = traverseSet(labels, i); 
            std::map<std::string, int> babyMap = (*mommyMap)[currentLabel];
            std::set<std::string> words;
            for (auto i : babyMap) {
                string word = i.first; //key
                // auto tally = i.second; //value
                words.insert(word);
            }
            double logLikelihood = 0;
            for(size_t w=0; w<words.size(); w++){
                
                string currentWord = traverseSet(words, w); 
                logLikelihood= calc_logLikeliehood((*postStats)[currentLabel],babyMap[currentWord]);
                cout<<"  "<<currentLabel<<":";
                cout<<currentWord<<", count = "<<babyMap[currentWord]<<", ";
                cout<<"log-likelihood = "<<logLikelihood<<endl;
            }
        }
    }

    void fillLabels(){
        for (auto &col:*postStats) {
            string label = col.first;
            labels.insert(label);
        }
    }
    
    //function that calculates log-prior for each label
    double calc_logPrior(int numPostsLabelC){
        double x = double (numPostsLabelC);
        double y = double (numTotalPosts);
        return log(x/y);
    }

    //function that calculates the log-likeliehood of a word in a given class
    double calc_logLikeliehood(int numPostsLabelC, int numPostsLabelCWordW){
        double x = double (numPostsLabelCWordW);
        double y = double (numPostsLabelC);
        return log(x/y);
    }

    double calc_logLikeliehoodTest(int numPostsLabelC, const string & word, const string & labelC){
        bool inAllWords=false;
        int totalWordCount = 0; 
        for (auto &i : *mommyMap) {
            std::map<std::string, int> babyMap = i.second; 
            for(auto b : babyMap){
                if(babyMap.count(word)>0){
                    inAllWords=true;
                }
            }
            totalWordCount += babyMap[word]; 
        }
        bool inLabelC = false;
        std::map<std::string, int> labelCMap = (*mommyMap)[labelC];
        if(labelCMap.count(word)>0){
            inLabelC=true;
        }
        if(inAllWords==false){
            double y = double (numTotalPosts); 
            return log(1.0/y);
        }
        else if(inAllWords==true && inLabelC==false){
            double x = double (totalWordCount);
            double y = double (numTotalPosts);
            return log(x/y);
        }
        else {
            return calc_logLikeliehood((*postStats)[labelC],labelCMap[word]);
        } 
    }

    //function that calculates log-probability score for a given post
    double calc_logProbability(set<string> postWords, string & mostLikelyLabel){
        double maxLogProbability=-100000;
        for(size_t i=0; i<labels.size(); i++){
            double currentLogProbability=0;
            string currentLabel = traverseSet(labels, i); 
            currentLogProbability += calc_logPrior((*postStats)[currentLabel]);
            for(size_t w = 0; w<postWords.size(); w++) {
                string currentUniqueWord = traverseSet(postWords, w); 
                currentLogProbability+=calc_logLikeliehoodTest((*postStats)[currentLabel], currentUniqueWord, currentLabel);
            }
            if(currentLogProbability>maxLogProbability){
                mostLikelyLabel=currentLabel;
                maxLogProbability=currentLogProbability;
            }
        }
        return maxLogProbability;
    }

    //function that prints out predictions based on test data
    void predictions(){
        numCorrect=0;
        numTestPosts=0;
        cout<<"test data:"<<endl;
        csvstream csvin2(testFileName);
        map<string, string> row;
        while (csvin2 >> row) {
            numTestPosts++;
            std::string correctLabel="";
            
            correctLabel=row["tag"];
            std::string predicted = "";
            std::set<string> postWords;
            istringstream ss(row["content"]);
            std::string word="";
            while(ss>>word){
                postWords.insert(word);
            }
            double logProb = calc_logProbability(postWords, predicted);
            cout<<"  correct = "<<row["tag"]<<", predicted = "<<predicted;
            cout<<", log-probability score = "<<logProb<<endl;
            cout<<"  content = "<<row["content"]<<endl;
            cout<<endl;
            if(correctLabel==predicted) numCorrect++;
        }
    }
    
    void printClasses() {
        cout<<"classes:"<<endl;
        for(auto i : *postStats) {
            std::string label = i.first; 
            int numPosts = i.second;
            cout<<"  "<<label<<", "<<numPosts<<" examples, log-prior = "<<calc_logPrior((*postStats)[label])<<endl; 
        }
    }

    //function that prints out total success rate 
    void printSuccessRate(){
        cout<<"performance: "<<numCorrect<<" / "<<numTestPosts<<" posts predicted correctly"<<endl;
    }

    //constructor
    Classifier(std::map<std::string, std::map<std::string, int>> * mommyMap_in, 
    std::map<std::string, int> * postStats_in, int numTotalPosts_in, string testFileName_in):
    mommyMap(mommyMap_in), postStats(postStats_in), numTotalPosts(numTotalPosts_in), testFileName(testFileName_in){
        numCorrect=0;
        numTestPosts=0;
    }

    //destructor
    ~Classifier() {
        delete mommyMap; 
    }

    private:
    //map of maps passed in from other class
    std::map<std::string, std::map<std::string, int>> * mommyMap;
    //variable that keeps track of correct predictions
    int numCorrect;
    //vector with label names
    std::set<std::string> labels;
    //map that keeps track of how many posts there are per label
    std::map<std::string, int> * postStats;
    //variable that stores total number of posts
    int numTotalPosts;
    //variable that stores number of test posts
    int numTestPosts;
    //fileName of testData
    std::string testFileName;
}; 

//Write classifier functions here 

int main(int argc, char* argv[]) {
    bool debug = false;
    string fourthArg = "";
    if(argc == 4) {
        fourthArg=argv[3];
        if(fourthArg == "--debug") {
            debug = true; 
        } else {
            return -1;
        }
    }
    if(argc!= 3 && argc!= 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }
    string trainingFileName = "";
    trainingFileName= argv[1]; 
    string testingFileName = "";
    testingFileName=argv[2];
    csvstream csvin(trainingFileName);
    csvstream csvin2(testingFileName);
    ReadCsv * train = new ReadCsv(trainingFileName);
    cout.precision(3);
    train->growMommyTree();
    train->counter();
    if(debug){
        train->trainingPrinter();
        cout<<"trained on "<<train->getNumPosts()<<" examples"<<endl;
        // cout<<endl;
        train->printVocabSize();
    } 
    else{
        cout<<"trained on "<<train->getNumPosts()<<" examples"<<endl;
        cout<<endl;
    }
    Classifier * test = new Classifier(train->get_mommyMap(), train->get_postStats(), 
        train->getNumPosts(), testingFileName);
    test->fillLabels(); 
    if(debug){
        test->printClasses(); 
        test->printClassifierParameters();
    }
    test->predictions(); 
    test->printSuccessRate();
    return 0;    
}