#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "huffman.h"

typedef struct _BitCode{
	uint8_t bits;
	int num_bits;
} BitCode;

typedef BitCode Code[256];

static int _cmp_freq(const void* o1, const void* o2);
static void _make_table(TreeNode* root, Code code_table, BitCode ch);

static int _cmp_freq(const void* o1, const void* o2) {
	int diff = (const int)(((TreeNode*)o1) -> frequency) - (const int)(((TreeNode*)o2) -> frequency);
	if(diff == 0) {
		if((const char)(((TreeNode*)o1) -> character) == '\0' || (const char)(((TreeNode*)o2) -> character) == '\0') {
			diff = -1;
		}
		else {
			diff = (const int)(((TreeNode*)o1) -> character) - (const int)(((TreeNode*)o2) -> character);
		}
	}
	return diff;
}

Node* make_huffman_pq(Frequencies freqs) {
	Node* head = NULL;
	for(int f_idx = 0; f_idx < 256; f_idx++) {
		if(freqs[f_idx] > 0) {
			TreeNode* bst_node = malloc(sizeof(*bst_node));
			uchar ch = f_idx;
			bst_node -> character = ch;
			bst_node -> frequency = freqs[f_idx];
			bst_node -> left = NULL;
			bst_node -> right = NULL;
			pq_enqueue(&head, bst_node, _cmp_freq);
		}
	}
	return head;
}

TreeNode* make_huffman_tree(Node* head) {
	if(head == NULL) {
		return NULL;
	}
	while(head -> next != NULL) {
		Node* pop1 = pq_dequeue(&head);
		size_t pop1_freq = ((TreeNode*)(pop1 -> a_value)) -> frequency;
		Node* pop2 = pq_dequeue(&head);
		size_t pop2_freq = ((TreeNode*)(pop2 -> a_value)) -> frequency;
		TreeNode* new_treenode = malloc(sizeof(*new_treenode));
		new_treenode -> character = '\0';
		new_treenode -> frequency = pop1_freq + pop2_freq;
		new_treenode -> left = pop1 -> a_value;
		new_treenode -> right = pop2 -> a_value;
		pq_enqueue(&head, new_treenode, _cmp_freq);
		free(pop1);
		free(pop2);
	}
	TreeNode* tree_root = head -> a_value;
	free(head);
	return tree_root;
}

void destroy_huffman_tree(TreeNode** a_root) {
	if(*a_root != NULL) {
		destroy_huffman_tree(&((*a_root) -> left));
		destroy_huffman_tree(&((*a_root) -> right));
		free(*a_root);
		*a_root = NULL;
	}
}

void write_coding_table(TreeNode* root, BitWriter* a_writer) {
	if(root != NULL) {
		write_coding_table(root -> left, a_writer);
		write_coding_table(root -> right, a_writer);
		if(root -> character) {
			write_bits(a_writer, 0x01, 1);
			write_bits(a_writer, root -> character, 8);
		}
		else {
			write_bits(a_writer, 0, 1);
		}
	}
}

void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t* uncompressed_bytes) {
	Code code_table = {0};
	BitCode non_char = { .bits = 0, .num_bits = 0 };
	_make_table(root, code_table, non_char);
	for(int idx = 0; uncompressed_bytes[idx] != '\0'; idx++) {
		write_bits(a_writer, (code_table[uncompressed_bytes[idx]]).bits, (code_table[uncompressed_bytes[idx]]).num_bits);
	}
}

static void _make_table(TreeNode* root, Code code_table, BitCode ch) {
	if(root -> left == NULL && root -> right == NULL) {
		code_table[root -> character] = ch;
	}
	else {
		ch.bits = ch.bits << 1;
		ch.num_bits += 1;
		_make_table(root -> left, code_table, ch);
		ch.bits += 1;
		_make_table(root -> right, code_table, ch);
	}
}
