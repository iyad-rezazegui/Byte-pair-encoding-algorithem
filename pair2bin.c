#include<stdio.h>
#include<stdio.h>
#include<string.h>
#include <stdint.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include"nob.h"
#define STB_DS_IMPLEMENTATION
#include"stb_ds.h"
#include <stdlib.h>

typedef struct{
  uint32_t l,r;
}Pair;
typedef struct{
  Pair *items ;
  size_t count ;
  size_t capacity ;
}Pairs ;

typedef struct{
  Pair key ;
  size_t value;
}token;
typedef struct{
  uint32_t *items ;
  size_t count ;
  size_t capacity ;
}Tokens;
void render_tokens(Tokens token , Pairs pair){
  for(int i=0;i<token.count;++i){
    assert(token.items[i]<pair.count);
      if(pair.items[token.items[i]].l==token.items[i]){
	printf("%c" ,token.items[i]);
      }else {
        printf("[%u]",token.items[i]);
       }
 
  }
  printf("\n");
}



#define da_swap(Type,x,y)			\
  do{ Type t= (x) ;				\
  (x) = (y) ; \
   (y) = t ;\
  }while(0)


bool dump_pairs(const char *file_path,Pairs *pairs){
  return write_entire_file(file_path,pairs->items,sizeof(*pairs->items)*pairs->count);
}

bool load_pairs(const char *file_path , Pairs *pairs,String_Builder *sb){
  
  if(!nob_read_entire_file(file_path,sb)) return false ;

  if(sb->count % sizeof(*pairs->items) !=0) return false ;
  Pair *items = (void*)sb->items;
  size_t items_count = sb->count / sizeof(*pairs->items);
  for(int i=0;i<items_count;++i){
    da_append(pairs,items[i]);
  }
  return true ;
  
  }

int main(void){
  char *text = "The concept of compiling the world's knowledge in a single location dates back to the ancient Library of Alexandria and Library of Pergamum, and there are ancient precursors of the idea of a comprehensive encyclopedia, such as Pliny the Elder's Naturalis historia, but the modern concept of a general-purpose, widely distributed, printed encyclopedia originates with Denis Diderot and the 18th-century French encyclopedists.[10] The idea of using automated machinery beyond the printing press to build a more useful encyclopedia can be traced to Paul Otlet's 1934 book Traité de Documentation. Otlet also founded the Mundaneum, an institution dedicated to indexing the world's knowledge, in 1910. This concept of a machine-assisted encyclopedia was further expanded in H. G. Wells' book of essays World Brain (1938) and Vannevar Bush's future vision of the microfilm-based Memex in his essay As We May Think (1945).[11] Another milestone was Ted Nelson's hypertext design Project Xanadu, which began in 1960.[11]";
  // we fill our token array
  Tokens tokens_in={0};
size_t len = strlen(text);
for (size_t i = 0; i < len; ++i) {
  da_append(&tokens_in, (unsigned char)text[i]);
 } // here we must use unsigned char since we might have some wierd char
  // we need to fill the array of pairs we have
  Pairs pairs ={0};
  for(int  i=0;i<256;++i){
    Pair pair = {.l=i};
    da_append(&pairs,pair);
  }
  for(;;){
  // we need to count the frequencies in our hash table
  token  *freqs = NULL;
  
       for(ptrdiff_t i=0 ;i+1<tokens_in.count;++i){
    // we look if pair is in the table and that its value
 Pair pair = {.l=tokens_in.items[i],.r=tokens_in.items[i+1]};
       ptrdiff_t t =  hmgeti(freqs,pair);
            if (t<0)  hmput(freqs, pair, 1);
           else freqs[t].value+=1; // the mistake i made here is i was using i as an index and i was supposed to use the key instead .    
 }
  int max_index=0;
    // we look for the maximum pair freq
          for(ptrdiff_t i=1;i<hmlen(freqs);++i){
                if(freqs[max_index].value<freqs[i].value)
	         max_index=i;
    }
                if(freqs[max_index].value <=1) break ;
                Tokens tokens_out={0};
		da_append(&pairs,freqs[max_index].key);
    
          for(int i=0;i<tokens_in.count;){
      // first case we reached the end
        if (i+1>=tokens_in.count){ // we did plus one since were looking at pairs .
	da_append(&tokens_out,tokens_in.items[i]);
      break;
      }
    
    
            else {
        Pair pair = {.l=tokens_in.items[i],.r=tokens_in.items[i+1]};
	if(memcmp( &pair, &freqs[max_index].key,sizeof(pair))==0){
	da_append(&tokens_out,pairs.count-1);
	i +=2;
	}
	else{
	da_append(&tokens_out,tokens_in.items[i]);
	i++;
	}
    }
	  }
    da_swap(Tokens,tokens_in,tokens_out);
     render_tokens(tokens_in,pairs);
    tokens_out.count=0;
  }
  return 0 ;
}
