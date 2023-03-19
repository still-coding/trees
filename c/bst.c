#include "bst.h"

static void output_nodes_to_dotfile(tree_node_t *node, FILE *file, print_datatype_func print_data)
{
    if (!node)
        return;
    fprintf(file, "%zu [label=<<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\"><TR>\n", (size_t)node);
    if (node->left)
        fprintf(file, "<TD port=\"left\"><SUB><i>l</i></SUB></TD>\n"); 
    fprintf(file, "<TD CELLPADDING=\"7\"><b>");
    print_data(file, node->data);
    fprintf(file, "</b></TD>\n");
    if (node->right)
        fprintf(file, "<TD port=\"right\"><SUB><i>r</i></SUB></TD>\n");
    fprintf(file, "</TR></TABLE>>]\n");
    if (node->left)
        fprintf(file, "%zu:left -> %zu\n", (size_t)node, (size_t)node->left);
    if (node->right)
        fprintf(file, "%zu:right -> %zu\n", (size_t)node, (size_t)node->right);
    output_nodes_to_dotfile(node->left, file, print_data);
    output_nodes_to_dotfile(node->right, file, print_data);
}

void bst_render(bst_t *bst, print_datatype_func print_data, bool open)
{
    struct stat st = {0};
    const char *pics_folder = "./pics";
    if (stat(pics_folder, &st) == -1)
        if (mkdir(pics_folder, 0777) == -1)
        {
            printf("Error: Failed to create folder '%s'\n", pics_folder);
            exit(EXIT_FAILURE);
        }

    char *filename = calloc(64, 1);
    sprintf(filename, "%s/bst_%zu",pics_folder, (size_t)bst);
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error: unable to open file '%s' for writing\n", filename);
        free(filename);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "digraph bst_%zu {\nnode [shape=ellipse]\n", (size_t)bst);
    output_nodes_to_dotfile(bst->root, file, print_data);
    fprintf(file, "}\n");
    fclose(file);
    char *command = calloc(64, 1);
    sprintf(command, "dot -Tpng %s -o %s.png", filename, filename);
    system(command);
    remove(filename);
    free(command);
    if (open)
    {
        command = calloc(64, 1);
        sprintf(command, "xdg-open %s.png", filename);
        system(command);
        free(command);
    }
    free(filename);
}