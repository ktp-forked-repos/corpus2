#ifndef TREE_H
#define TREE_H


#include <map>

#include <boost/filesystem.hpp>

#include <libcorpus2/token.h>
#include <libcorpus2/guesser/backwardtrie.h>



struct Properties
{
	int depth;  /// Distance from root.
	int counter;  /// How many words pass throught this letter.
	
	typedef std::pair<UnicodeString, int> recipe;
	struct tag_info
	{
		int strength;  /// How often was it used
		std::map<recipe, int> recipes;
		
		tag_info(){strength = 0;}
	};

	typedef std::map <Corpus2::Tag,	tag_info> tag_map;
	tag_map tags;
	
	Properties();
	void feedOn(Properties &another);
	
	
	void dump(std::ostream& stream) const;
	void print(std::ostream& stream) const;
};




class Tree : public Corpus2::BackwardTrie<Properties>
{
public:
	
	/**
	  Insert a word.
	  */
	void insert(const Corpus2::Token & token);
	
	/**
	  Compress losslessly the tree, removing all nodes used only once, bar first ones.
	  */
	void compressLemmawise();
	
	/**
	  Moves all tags upward if they are similar.
	  */
	void gatherTags();
	
	/**
	  Guess tags for all empty nodes using its descendants
	  */
	void guessEmpty();
	
	/**
	  Remove all nodes that are not intersting.
	  */
	void prune();
	
	
private:
	static bool compressLemmawise(Node & node);
	
	/// Remove all branches without any tags.
	static bool compressTagwise(Node & node);
	
	static void gatherTags(Node & node);
	
	static void guessEmpty(Node & node);
	
	void prune(Node & node) const;
	bool isBad(const Node &node) const;
	static void collect(Node & node);
	
	
	static bool isSup(const Corpus2::Token & token);
	static bool isNeg(const Corpus2::Token & token);
	
	/// Unmarks lexeme (removes 'naj' and 'nie' from lemma, changes tag) and cuts lemma
	/// so that it no longer contain colons and thingies after them.
	static Corpus2::Lexeme normalize(const Corpus2::Lexeme & lexeme);
};


#endif // TREE_H
