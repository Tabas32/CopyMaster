int error_description(int ret_val, void (*errorType)(char* option), char* option);

void file_missing_error(char* option);
void other_error(char* option);
void options_error(char* option);
void notDir_error(char* option);
void position_error(char* option);
void permissions_error(char* option);
void file_type_error(char* option);
void inode_error(char* option);
void file_exists_error(char* option);
void cannot_create_error(char* option);
void notdeleted_error(char* option);
void notchanged_error(char* option);
