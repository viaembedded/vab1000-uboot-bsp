
struct drive_str {
    char *rw_reg;
    unsigned int mask;
    unsigned int offset;
    unsigned int w_reg;
};

void fill_driving_value(unsigned int base_addr, unsigned int driving_map_size, struct drive_str *driving_map){

	unsigned int temp;
	unsigned int value;
	int map_index;
	//Note: 0x64=100, 0x65=101, 0x66=102, 0x67=103, 0x68=104
	//printf("Begin writing driving value\n");
	printf("Driving_map size is  %d\n",driving_map_size);
	for( map_index=0; map_index < driving_map_size/sizeof(driving_map[0]); map_index++){
		
		if (driving_map[map_index].rw_reg =="write"){

			temp= readb((void *)base_addr + driving_map[map_index].offset);
			//printf("readb %d is %d\n", driving_map[map_index].offset, temp);
			
			value=(temp & ~(driving_map[map_index].mask)) | driving_map[map_index].w_reg;
			writeb(value, (void *)base_addr + driving_map[map_index].offset);
			//printf("writeb %d is %d\n", driving_map[map_index].offset, value);
	
		}else if(driving_map[map_index].rw_reg =="end"){

			printf("loop is done\n");
			break;
			
		}else{
			printf("no execute");
		}
	}
	//printf("Finish writing driving value\n");

}
