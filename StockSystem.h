
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBER 20

#define true 1
#define false 1

//represent a fashion
typedef struct FashionItem
{
	int id;
}item;

//the current volume of a fashion item in the stock
typedef struct ItemData
{
	struct FashionItem item;
	int Volume_in_Stock; //the quantity of the item in stock.
	int Sold; // the sold quantity of the item
	int Returned; // the returned quantity of the item
}itemdata;

typedef struct TreeNode
{
	struct ItemData *data;
	struct TreeNode *left;
	struct TreeNode *right;
}treenode;

//Binary Search Tree
void Insert(struct TreeNode **root, struct ItemData *data);
struct TreeNode* Query(struct TreeNode *root, int id);

//All the quantities are zero initially
void Buy (int id, int buyQ);
int Sell (int id, int soldQ);
int Return (int id, int returnQ);
void BestSOLD();
void SoldList();
