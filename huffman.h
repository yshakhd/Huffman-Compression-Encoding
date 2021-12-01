#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdlib.h>
#include "frequencies.h"
#include "priority_queue.h"
#include "bit_writer.h"

typedef struct _TreeNode {
	uchar character;
	size_t frequency;
	struct _TreeNode* left;
	struct _TreeNode* right;
} TreeNode;

Node* make_huffman_pq(Frequencies freqs);
TreeNode* make_huffman_tree(Node* head);
void destroy_huffman_tree(TreeNode** a_root);
void write_coding_table(TreeNode* root, BitWriter* a_writer);
void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t* uncompressed_bytes);

#endif

#define _HUFFMAN_H_V1
