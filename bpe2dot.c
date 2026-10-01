#include<stdio.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include"nob.h"
#include"bpe.h"
bool load_pairs(const char *file_path , Pairs *pairs,String_Builder *sb){
  
  if(!nob_read_entire_file(file_path,sb)) return 0 ;
  if(sb->count % sizeof(*pairs->items)!=0) {
    fprintf(stderr,"ERROR: size of %s must be devisible by : %zu",file_path,sizeof(pairs->items));
    return 0; 
  }
  Pair *items = (void*)sb->items;
  size_t items_count=sb->count/sizeof(*pairs->items);
  for(size_t i=0;i<items_count;++i){
    nob_da_append(pairs,items[i]);
  }
  return 1;
}

void render_dot(Pairs pairs,String_Builder *sb){
  sb_append_cstr(sb,"digraph Pairs {\n");
  for(uint32_t i =0; i<pairs.count;++i){
    if(i != pairs.items[i].l){
      sb_append_cstr(sb, temp_sprintf("%u -> %u,%u\n",i,pairs.items[i].l,pairs.items[i].r));  
    }
  }
  sb_append_cstr(sb,"}\n");
}


int main(int argc ,char **argv)
{
  const char *program_name = shift(argv, argc);
    if(argc <=0){
    fprintf(stderr,"ERROR : u should have an input ");
  }
  const char *file_path_input = shift(argv,argc);
  if(argc <=0){
    fprintf(stderr,"ERROR : u should have an input ");
  }
  const char *file_path_output=shift(argv,argc);
  String_Builder sb = {0};
  Pairs pairs = {0};
  if(!load_pairs( file_path_input , &pairs,&sb)) return 1 ;
  sb.count=0;
  render_dot(pairs,&sb);
  if(!write_entire_file(file_path_output, sb.items,sb.count)) return 1 ;
  return 0 ; 
}

