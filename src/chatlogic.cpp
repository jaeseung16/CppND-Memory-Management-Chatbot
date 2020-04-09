#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE
    ////

    // create instance of chatbot
    _chatBot = new ChatBot("../images/chatbot.png");

    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    _chatBot->SetChatLogicHandle(this);

    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE
    ////

    // delete chatbot instance
    _chatBot->SetChatLogicHandle(nullptr);

    // delete all nodes
    //for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    //{
    //    delete *it;
    //}

    //_panelDialog = nullptr;
    
    ////
    //// EOF STUDENT CODE
}

ChatLogic::ChatLogic(const ChatLogic &source)
{
    std::cout << "COPYING content of instance " << &source << " to instane " << this << std::endl;

    for (auto it = std::begin(source._nodes); it != std::end(source._nodes); ++it)
    {
        GraphNode node = *(*it);
        _nodes.push_back(std::move(std::make_unique<GraphNode> (node.GetID())));
    }

    _currentNode = source._currentNode;
    _chatBot = source._chatBot;
    _panelDialog = source._panelDialog;
}
    
ChatLogic& ChatLogic::operator=(const ChatLogic &source)
{
    std::cout << "ASSIGNING content of instance " << &source << " to instane " << this << std::endl;
    
    if (this == &source)
        return *this;

    _currentNode = nullptr;
    _chatBot = nullptr;
    _panelDialog = nullptr;

    _nodes.clear();
    
    for (auto it = std::begin(source._nodes); it != std::end(source._nodes); ++it)
    {
        GraphNode node = *(*it);
        _nodes.push_back(std::move(std::make_unique<GraphNode> (node.GetID())));
    }
    
    _currentNode = source._currentNode;
    _chatBot = source._chatBot;
    _panelDialog = source._panelDialog;

    return *this;
}
    
ChatLogic::ChatLogic(ChatLogic &&source)
{
    std::cout << "MOVING (constructor) content of instance " << &source << " to instane " << this << std::endl;
    
    for (auto it = std::begin(source._nodes); it != std::end(source._nodes); ++it)
    {
        std::unique_ptr<GraphNode> node = std::move(*it);
        _nodes.push_back(std::move(node));
    }
    
    _currentNode = source._currentNode;
    _chatBot = source._chatBot;
    _panelDialog = source._panelDialog;

    source._currentNode = nullptr;
    source._chatBot = nullptr;
    source._panelDialog = nullptr;
}
    
ChatLogic & ChatLogic::operator=(ChatLogic &&source)
{
    std::cout << "MOVING (assignment) content of instance " << &source << " to instane " << this << std::endl;
    
    if (this == &source)
        return *this;
    
    _currentNode = nullptr;
    _chatBot = nullptr;
    _panelDialog = nullptr;

    _nodes.clear();

    for (auto it = std::begin(source._nodes); it != std::end(source._nodes); ++it)
    {
        std::unique_ptr<GraphNode> node = std::move(*it);
        _nodes.push_back(std::move(node));
    }

    _currentNode = source._currentNode;
    _chatBot = source._chatBot;
    _panelDialog = source._panelDialog;

    source._currentNode = nullptr;
    source._chatBot = nullptr;
    source._panelDialog = nullptr;

    return *this;
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE
                        ////

                        // check if node with this ID exists already
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode>& node) { return node->GetID() == id; });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            _nodes.push_back(std::move(std::make_unique<GraphNode> (id)));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode>& node) { return node->GetID() == std::stoi(parentToken->second); });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](std::unique_ptr<GraphNode>& node) { return node->GetID() == std::stoi(childToken->second); });

                            // create new edge
                            std::unique_ptr<GraphEdge> edge (new GraphEdge(id));
                            edge->SetChildNode((*childNode).get());
                            edge->SetParentNode((*parentNode).get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get());
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                rootNode = it->get(); // assign current node to root
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    // add chatbot to graph root node
    _chatBot->SetRootNode(rootNode);
    //rootNode->MoveChatbotHere(_chatBot);
    
    std::unique_ptr<ChatBot> chatBotInStack (_chatBot);
    chatBotInStack -> SetChatLogicHandle(this);
    
    //std::cout << "chatbot = " << chatBotInStack << std::endl;
    //std::cout << "_chatbot = " << _chatBot << std::endl;
    rootNode->MoveChatbotHere(std::move(chatBotInStack));
    //std::cout << "rootNode = " << rootNode << std::endl;
    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
