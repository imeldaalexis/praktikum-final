#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void fsInit() {
  struct map_fs map_fs_buf;
  int i = 0;

  readSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
  for (i = 0; i < 16; i++) map_fs_buf.is_used[i] = true;
  for (i = 256; i < 512; i++) map_fs_buf.is_used[i] = true;
  writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
}

// TODO: 2. Implement fsRead function
void fsRead(struct file_metadata* metadata, enum fs_return* status) {
  struct node_fs node_fs_buf;
  struct data_fs data_fs_buf;

  int i, j;

  readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER);

//mencari node yang sesuai dengan for loop
    for (i = 0; i < FS_NODE_COUNT; i++) {
        if (node_fs_buf.nodes[i].parent_node_index == metadata->parent_index &&
            strcmp(node_fs_buf.nodes[i].name, metadata->node_name) == 0) {

            // jika node ditemukan  dan node adalah direktori
            if (node_fs_buf.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                // ganti status
                *status = FS_R_TYPE_IS_DIRECTORY;
                return;
            }

            // jika node adalah file, melewati pencarian folder tadi , terus masuk ke pencarian sektor
            metadata->filesize = 0;
            for (j = 0; j < FS_MAX_SECTOR; j++) {
                if (node_fs_buf.nodes[i].sector_entry_index[j] == 0x00) {
                    // jika index adalah 0x00, stop loop
                    break;
                }
                readSector(metadata->buffer + (j * SECTOR_SIZE), node_fs_buf.nodes[i].sector_entry_index[j]);
                metadata->filesize += SECTOR_SIZE;
            }
            *status = FS_R_SUCCESS;
            return;
        }
    }

    // jika node tidak ditemukan
    *status = FS_R_NODE_NOT_FOUND;
}

// TODO: 3. Implement fsWrite function
void fsWrite(struct file_metadata* metadata, enum fs_return* status) {
  struct map_fs map_fs_buf;
  struct node_fs node_fs_buf;
  struct data_fs data_fs_buf;
}
