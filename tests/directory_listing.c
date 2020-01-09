#include "../directory_listing.c"

int main(){
	directory_listing("/");
	directory_listing(".");
	directory_listing("www");
}
