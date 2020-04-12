#include <iostream>

#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////

    ////
    //// EOF STUDENT CODE
}

// rule of five: copy/move constructors & assigment operators
GraphNode::GraphNode(const GraphNode &source)
{
    std::cout << "COPYING content of instance " << &source << " to instane " << this << std::endl;

    for (auto it = std::begin(source._childEdges); it != std::end(source._childEdges); ++it)
    {
        GraphEdge childEdge = *(*it);
        _childEdges.push_back(std::move(std::make_unique<GraphEdge> (childEdge.GetID())));
    }

    for (auto it = std::begin(source._parentEdges); it != std::end(source._parentEdges); ++it)
    {
        _parentEdges.push_back(std::move(*it));
    }

    _chatBot = source._chatBot;
    _id = source._id;

    for (auto it = std::begin(source._answers); it != std::end(source._answers); ++it)
    {
        std::string answer = *it;
        _answers.push_back(answer);
    }
}

GraphNode & GraphNode::operator=(const GraphNode &source)
{
    std::cout << "ASSIGNING content of instance " << &source << " to instane " << this << std::endl;
    
    if (this == &source)
        return *this;
    
    for (auto it = std::begin(source._childEdges); it != std::end(source._childEdges); ++it)
    {
        GraphEdge childEdge = *(*it);
        _childEdges.push_back(std::move(std::make_unique<GraphEdge> (childEdge.GetID())));
    }

    for (auto it = std::begin(source._parentEdges); it != std::end(source._parentEdges); ++it)
    {
        _parentEdges.push_back(*it);
    }

    _chatBot = source._chatBot;
    _id = source._id;

    for (auto it = std::begin(source._answers); it != std::end(source._answers); ++it)
    {
        std::string answer = *it;
        _answers.push_back(answer);
    }

    return *this;
}

GraphNode::GraphNode(GraphNode &&source)
{
    std::cout << "MOVING (constructor) content of instance " << &source << " to instane " << this << std::endl;

    for (auto it = std::begin(source._childEdges); it != std::end(source._childEdges); ++it)
    {
        _childEdges.push_back(std::move(*it));
    }

    for (auto it = std::begin(source._parentEdges); it != std::end(source._parentEdges); ++it)
    {
        _parentEdges.push_back(*it);
    }

    _id = source._id;

    for (auto it = std::begin(source._answers); it != std::end(source._answers); ++it)
    {
        _answers.push_back(*it);
    }

    _chatBot = std::move(source._chatBot);
    source._childEdges.clear();
    source._parentEdges.clear();
    source._id = 0;
    source._answers.clear();
}
    
GraphNode & GraphNode::operator=(GraphNode &&source)
{
    std::cout << "MOVING (assignment) content of instance " << &source << " to instane " << this << std::endl;
    
    if (this == &source)
        return *this;

    _childEdges.clear();
    _parentEdges.clear();
    _answers.clear();
    
    for (auto it = std::begin(source._childEdges); it != std::end(source._childEdges); ++it)
    {
        _childEdges.push_back(std::move(*it));
    }

    for (auto it = std::begin(source._parentEdges); it != std::end(source._parentEdges); ++it)
    {
        _parentEdges.push_back(*it);
    }

    _id = source._id;

    for (auto it = std::begin(source._answers); it != std::end(source._answers); ++it)
    {
        _answers.push_back(*it);
    }

    _chatBot = std::move(source._chatBot);
    source._childEdges.clear();
    source._parentEdges.clear();
    source._id = 0;
    source._answers.clear();

    return *this;
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    _childEdges.push_back(std::move(edge));
}

//// STUDENT CODE
////
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    return (_childEdges[index]).get();

    ////
    //// EOF STUDENT CODE
}