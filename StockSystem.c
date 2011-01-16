
#include "StockSystem.h"

//#define DEBUG

//Binary Search Tree
struct TreeNode *root=NULL;

int NodeOfNumber = 0;

void Insert(struct TreeNode **root, struct ItemData *data)
{
    if(*root==NULL)
    {
        *root=(struct TreeNode*) malloc( sizeof(struct TreeNode) );
        (*root)->data = data;
        (*root)->left=(*root)->right=NULL;
    }
    else
    {   
        if( data > (*root)->data )
		Insert(  &(*root)->right ,data);
        else 
		Insert(  &(*root)->left  ,data);
    }
}

struct TreeNode* Query(struct TreeNode *root, int id)
{
    if(root==NULL)
	return NULL;
    else
    {
        if((root->data)->item.id == id)
		return root;
        else if(root->data->item.id < id)
        	return Query(root->right, id);
        else 
		return Query(root->left, id);
    }
}

void DumpTree(struct TreeNode* root) 
{
    if (root != NULL) 
     {
        DumpTree(root->left);
	 printf("%d    %d                    %d        %d\n", 
	 root->data->item.id, root->data->Volume_in_Stock, root->data->Sold, root->data->Returned);
        DumpTree(root->right);
     }
}

void Buy (int id, int buyQ)
{
	//1. search 
	struct TreeNode *searchResult = Query(root, id);
	struct ItemData *idata;

	if(searchResult == NULL)
	{
		if (NodeOfNumber >= MAX_NUMBER)
		{
			#ifdef DEBUG
				printf("total number of items is less than a maximum number\n");
			#endif
			return ;
		}
		//2. not exist: create new id
		#ifdef DEBUG
			printf("not exist: create new node\n");
		#endif

		idata = (struct ItemData*) malloc( sizeof(struct ItemData) );
		NodeOfNumber++;
		idata->item.id = id;
		idata->Volume_in_Stock = buyQ;
		idata->Sold = 0;
		idata->Returned = 0;
		Insert(&root, idata);
	}
	else 
	{
		//Search it, refresh data
		searchResult->data->Volume_in_Stock = buyQ;
		#ifdef DEBUG
			printf("buy: exist, refresh data: %d %d\n", 
				searchResult->data->item.id, searchResult->data->Volume_in_Stock);
		#endif
	}

}

int Sell (int id, int soldQ)
{
	//1. search 
	struct TreeNode *searchResult = Query(root, id);

	if(searchResult == NULL)
	{
		if (NodeOfNumber >= MAX_NUMBER)
		{
			#ifdef DEBUG
				printf("total number of items is less than a maximum number\n");
			#endif
			return false;
		}
		//2. not exist: create new id
		#ifdef DEBUG
			printf("id %d: not exist, no Sell\n", id);
		#endif

		return false;
	}
	else 
	{
		//Search it, refresh data
		if (searchResult->data->Volume_in_Stock < soldQ)
		{
		#ifdef DEBUG
			printf("Sell:  %d < %d, not enough.\n", 
				searchResult->data->Volume_in_Stock, soldQ);
		#endif
			return false;
		}

		searchResult->data->Sold = soldQ;
		searchResult->data->Volume_in_Stock = 
				(searchResult->data->Volume_in_Stock - soldQ);

		#ifdef DEBUG
			printf("id %d: data exist, sell refresh data: Volume_in_Stock=%d, Sold=%d\n",
				searchResult->data->item.id, 
				searchResult->data->Sold,
				searchResult->data->Volume_in_Stock);
		#endif
	}

	return true;
}

int Return (int id, int returnQ)
{
	//1. search 
	struct TreeNode *searchResult = Query(root, id);

	if(searchResult == NULL)
	{
		if (NodeOfNumber >= MAX_NUMBER)
		{
			#ifdef DEBUG
				printf("total number of items is less than a maximum number\n");
			#endif
			return false;
		}
		//2. not exist: create new id
		#ifdef DEBUG
			printf("id %d: not exist, no Return\n", id);
		#endif

		return false;
	}
	else 
	{
		//Search it, refresh data
		#ifdef DEBUG
			printf("return: exist, refresh data: %d %d\n", 
				searchResult->data->item.id, searchResult->data->Volume_in_Stock);
		#endif

		if (searchResult->data->Sold < returnQ)
		{
		#ifdef DEBUG
			printf("return:  %d < %d, not enough.\n", 
				searchResult->data->Volume_in_Stock, returnQ);
		#endif
			return false;
			
		}
	
		searchResult->data->Sold = searchResult->data->Sold - returnQ;
		searchResult->data->Volume_in_Stock =  
					searchResult->data->Volume_in_Stock + returnQ;
		searchResult->data->Returned = returnQ;
		#ifdef DEBUG
		printf("id %d: data exist, return refresh data: Volume_in_Stock=%d, Sold=%d\n",
				searchResult->data->item.id, 
				searchResult->data->Sold,
				searchResult->data->Volume_in_Stock);
		#endif
	}

	return true;
}

void findBestSOLD(struct TreeNode *root)
{
    if (root != NULL) 
     {
        DumpTree(root->right);
     }
    else
     {
	printf("%d    %d                    %d        %d\n", 
	root->data->item.id, root->data->Volume_in_Stock, root->data->Sold, root->data->Returned);
	
     }
}


void BestSOLD()
{
	printf("BEST SOLD Report\n");
	printf("ID    VOLUME_IN_STOCK       SOLD      RETURN\n");
	printf("----- --------------------- --------- --------\n");
	findBestSOLD(root);
}

void SoldList()
{
	printf("SOLD Report\n");
	printf("ID    VOLUME_IN_STOCK       SOLD      RETURN\n");
	printf("----- --------------------- --------- --------\n");
	DumpTree(root);
}

/*
OPERATOR: one digit integer. 
	1 is for Buy function, 
	2 is for Sell function, 
	3 is for Return function;

D: 2 digits integer;

VOLUME: 3 digits integer.
*/
int main(int argc, char **argv)
{
	int operator = 0;
	int value;
	FILE *fp;
	char buffer[32]="";
	char digits[4]="";
	int length=0;
	int OPERATOR = 0, D = 0, VOLUME = 0;

	if (argc < 2) 
	{
		printf("./StockSystem.exe <filename>\n");
		return -1;
	}

	fp = fopen(argv[1],"r");

	while (!feof(fp))
	{
		fscanf(fp, "%s\n", buffer);
		length = strlen(buffer);
		if (length != 6)
		{
			printf("file: %s, format error.\n", argv[1]);
			return false;
		}

		//operator				
		digits[0] = buffer[0];
		digits[1] = '\0';
		OPERATOR = atoi(digits);

		//D
		digits[0] = buffer[1];
		digits[1] = buffer[2];
		digits[2] = '\0';
		D = atoi(digits);
				
		//VOLUME
		digits[0] = buffer[3];
		digits[1] = buffer[4];
		digits[2] = buffer[5];
		digits[3] = '\0';
		VOLUME = atoi(digits);

		#ifdef DEBUG
			printf("OPERATOR %d, D %d, VOLUME %d\n", OPERATOR, D, VOLUME);
		#endif

		//handler
		switch (OPERATOR)
		{
			case 1:
				Buy(D, VOLUME);
				break;
			case 2:
				Sell(D, VOLUME);
				break;
			case 3:
				Return(D, VOLUME);
				break;
			default:
				printf("no operator\n");
				break;
		}
	}

	BestSOLD();
	printf("\n\n");
	SoldList();

	fclose(fp);
	return 0;
}
