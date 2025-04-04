#ifndef IMG_CONFIG_H
#define IMG_CONFIG_H

#define IMG_ROWS_DEFAULT 200
#define IMG_COLS_DEFAULT 300

struct image_config
{
    int rows;
    int columns;

    image_config():
        rows(IMG_ROWS_DEFAULT),
        columns(IMG_COLS_DEFAULT)
    {};
    
    image_config(    
        int r, 
        int c
    ):
        rows(r),
        columns(c)
    {};
};

#endif //IMG_CONFIG_H
