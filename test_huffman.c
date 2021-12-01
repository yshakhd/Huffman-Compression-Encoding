#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "huffman.h"
#include "miniunit.h"

static unsigned char* _read_file(const char* path);

static void _destroy_node(void* node) {
	free(node);
}

static unsigned char* _read_file(const char* path) {
	FILE* file = fopen(path, "r");
	int ch_count = 0;
	//char ch;
	while(fgetc(file) != EOF) {
		ch_count++;
	}
	unsigned char* contents = malloc(sizeof(*contents) * (ch_count + 1));
	fseek(file, 0, SEEK_SET);
	for(int idx = 0; idx < ch_count; idx++) {
		contents[idx] = fgetc(file);
	}
	contents[ch_count] = '\0';
	fclose(file);
	return contents;
}

static void _print_tree(TreeNode const* root) {
	if(root != NULL) {
		_print_tree(root -> left);
		if(root -> character == '\n') {
			printf("\\n: %zu\n", root -> frequency);
		}
		else if(root -> character == '\t') {
			printf("\\t: %zu\n", root -> frequency);
		}
		else if(root -> character == '\0') {
			printf("\\0: %zu\n", root -> frequency);
		}
		else {
			printf("%c: %zu\n", root -> character, root -> frequency);
		}
		_print_tree(root -> right);
	}
}

int _test_huffman_valid() {
	mu_start();

	char* filename = "test.txt";
	Frequencies freqs = {0};
	const char* error;
	bool is_file = calc_frequencies(freqs, filename, &error);
	mu_check(is_file);
	Node* list = make_huffman_pq(freqs);
	mu_check(list != NULL);
	printf("Huffman Priority Queue\n");
	for(Node* curr = list; curr != NULL; curr = curr -> next) {
		if(((TreeNode*)(curr -> a_value)) -> character == '\n') {
			printf("\\n: %d\n", (int)(((TreeNode*)(curr -> a_value)) -> frequency));
		}
		else if(((TreeNode*)(curr -> a_value)) -> character == '\t') {
			printf("\\t: %d\n", (int)(((TreeNode*)(curr -> a_value)) -> frequency));
		}
		else if(((TreeNode*)(curr -> a_value)) -> character == ' ') {
			printf("[space]: %d\n", (int)(((TreeNode*)(curr -> a_value)) -> frequency));
		}
		else {
			printf("%c: %d\n", ((TreeNode*)(curr -> a_value)) -> character, (int)(((TreeNode*)(curr -> a_value)) -> frequency));
		}
	}
	
	unsigned char* file_contents = _read_file("test.txt");

	TreeNode* huffman_tree = make_huffman_tree(list);
	mu_check(huffman_tree != NULL);
	printf("Huffman Tree\n");
	_print_tree(huffman_tree);
	
	BitWriter writer1 = open_bit_writer("test.bits");
	write_coding_table(huffman_tree, &writer1);
	close_bit_writer(&writer1);
	
	BitWriter writer2 = open_bit_writer("compressed.bits");
	write_compressed(huffman_tree, &writer2, file_contents);
	close_bit_writer(&writer2);

	free(file_contents);
	destroy_huffman_tree(&huffman_tree);
	
	mu_end();
}

int _test_huffman_invalid() {
	mu_start();

	char* filename = "one.txt";
	Frequencies freqs = {0};
	const char* error;
	bool is_file = calc_frequencies(freqs, filename, &error);
	mu_check(is_file);
	freqs['\n']--;
	Node* list = make_huffman_pq(freqs);
	mu_check(list == NULL);
	
	TreeNode* huffman_tree = make_huffman_tree(list);
	mu_check(huffman_tree == NULL);

	destroy_huffman_tree(&huffman_tree);
	
	mu_end();
}

int main() {
	mu_run(_test_huffman_valid);
	mu_run(_test_huffman_invalid);

	return EXIT_SUCCESS;
}
