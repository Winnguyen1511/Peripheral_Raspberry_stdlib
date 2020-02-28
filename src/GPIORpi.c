#include "GPIORpi.h"

int GPIO_Init_Default(GPIO_t* instance, int number)
{
    //Default init name, gpio_number, 
    //auto export the gpio
    //default set at output
    //default set value to HIGH
    instance->export_status = UNEXPORTED;
    instance->active_low = HIGH_ACTIVE;
    instance->value = HIGH;
    char name[MAX_NAME_SIZE], number_char[MAX_PIN_SIZE];
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", number);
    strcat(name, number_char);
    //instance->name = (char*)malloc(MAX_NAME_SIZE *(sizeof(char)));
    instance->name = (char*)malloc(MAX_NAME_SIZE *(sizeof(char)));
    instance->path = (char*)malloc(MAX_PATH_SIZE* sizeof(char));

    strcpy(instance->path, GPIO_PATH);
    strcpy(instance->name, name);
    strcat(instance->path, instance->name);
    strcat(instance->path, VALUE);

    if(!GPIO_export(instance))
    {
        printf("Error: Cannot export gpio%d\n", instance->gpio_num);
        return ERROR;
    }

    if(!GPIO_set_direction(instance, OUTPUT))
    {
        printf("Error: Cannot set direction gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    if(!GPIO_set_edge(instance, NONE))
    {
        printf("Error: Cannot set edge gpio%d\n", instance->gpio_num);
        return ERROR;
    }

    if(!GPIO_set_value(instance, HIGH))
    {
        printf("Error: Cannot set value gpio%d\n", instance->gpio_num);
        return ERROR;
    }

    return SUCCESS;
    
}
int GPIO_Init_Custom(GPIO_t* instance,\
            int number, direction_t dir,\
            active_low_t act, edge_t ed, gpio_value_t initVal)
{
    instance->export_status = UNEXPORTED;
    instance->active_low = HIGH_ACTIVE;
    instance->value = HIGH;
    //Init name, gpio_number
    //init all other properties 
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", number);
    strcat(name, number_char);
    instance->name = (char*)malloc(MAX_NAME_SIZE *(sizeof(char)));
    instance->path = (char*)malloc(MAX_PATH_SIZE* sizeof(char));
    strcpy(instance->path, GPIO_PATH);
    strcpy(instance->name, name);
    strcat(instance->path, instance->name);
    strcat(instance->path, VALUE);
    instance->gpio_num = number;

    if(!GPIO_export(instance))
    {
        printf("Error: Cannot export gpio%d\n", instance->gpio_num);
        return ERROR;
    }

    if(!GPIO_set_active_low(instance, act))
    {
        printf("Error: Cannot set active low->%d gpio%d\n",act, instance->gpio_num);
        return ERROR;
    }

    if(!GPIO_set_direction( instance, dir ))
    {
        printf("Error: Cannot set direction gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    //printf("Init edge=%d\n", ed);
    if(!GPIO_set_edge(instance, ed))
    {
        printf("Error: Cannot set edge gpio%d\n", instance->gpio_num);
        return ERROR;
    }

    if(!GPIO_set_value(instance, initVal))
    {
        printf("Error: Cannot set value gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}




int GPIO_Denit(GPIO_t* instance)
{
    if(instance->direction == OUTPUT)
    {
        active_low_t tmpAct;
        if(!GPIO_get_active_low(instance, &tmpAct))
        {
            printf("Error: Cannot get active low gpio%d\n", instance->gpio_num);
            return ERROR;
        }

        if(tmpAct == HIGH_ACTIVE)
        {
            if(!GPIO_set_value(instance, LOW))
            {
                printf("Error: Cannot set value gpio%d\n", instance->gpio_num);
                return ERROR;
            }
        }
        else if(tmpAct == LOW_ACTIVE)
        {
            if(!GPIO_set_value(instance, HIGH))
            {
                printf("Error: Cannot set value gpio%d\n", instance->gpio_num);
                return ERROR;
            }
        }
        else
        {
            //do nothing
        }
    }
    if(!GPIO_unexport(instance))
    {
        printf("Error: Cannot unexport gpio%d", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}
int GPIO_export(GPIO_t* instance)
{
    //Export the gpio
    //if ioctl_cmd() return SUCCESS, then return SUCCESS
    //else print error log, return ERROR
    if(!ioctl_cmd(instance, EXPORT_CMD, NULL))
    {
        printf("Error: ioctl cmd export gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    //instance->export_status = EXPORTED;
    return SUCCESS;
}
int GPIO_unexport(GPIO_t* instance)
{
    //Export the gpio
    //Call ioctl_cmd(), pass the GPIO struct to ioctl_cmd, pass cmd UNEXPORT_CMD
    //if ioctl_cmd() return SUCCESS, then return SUCCESS
    //else print error log, return ERROR
    if(!ioctl_cmd(instance, UNEXPORT_CMD, NULL))
    {
        printf("Error: ioctl cmd unexport gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    //instance->export_status = UNEXPORTED;
    return SUCCESS;
}

int GPIO_set_active_low(GPIO_t* instance, active_low_t act)
{
    //Set gpio active low -> TRUE/FALSE
    //Call ioctl_cmd(), pass the GPIO struct to ioctl_cmd(), pass cmd SET_ACTIVE_LOW_CMD
    //Pass the active low value
    //if ioctl_cmd() return SUCCESS, then return SUCCESS
    //else print error log, return ERROR
    if(!ioctl_cmd(instance, SET_ACTIVE_LOW_CMD, &act))
    {
        printf("Error: ioctl cmd set active low gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    //instance->active_low = act;
    return SUCCESS;
}
int GPIO_set_value(GPIO_t* instance, gpio_value_t val)
{
    //Set value to LOW/HIGH
    //Call ioctl_cmd(), pass the GPIO struct to ioctl_cmd(), pass cmd SET_VALUE_CMD
    //Pass the gpio_value
    //if ioctl_cmd() return SUCCESS, then return SUCCESS
    //else print error log, return ERROR
    if(!ioctl_cmd(instance, SET_VALUE_CMD, &val))
    {
        printf("Error: ioctl cmd set value gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    //instance->value = val;
    return SUCCESS;
}



int GPIO_set_direction(GPIO_t* instance, direction_t dir)
{
    if(!ioctl_cmd(instance, SET_DIR_CMD, &dir))
    {
        printf("Error: ioctl cmd set direction gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    //instance->direction = dir;
    return SUCCESS;   
}
int GPIO_set_edge(GPIO_t* instance, edge_t ed)
{
    if(!ioctl_cmd(instance, SET_EDGE_CMD, &ed))
    {
        printf("Error: ioctl cmd set edge gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}

int GPIO_get_edge(GPIO_t* instance, edge_t* retVal)
{
    if(!ioctl_cmd(instance, GET_EDGE_CMD, retVal))
    {
        printf("Error: ioctl cmd get edge gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}

int GPIO_get_active_low(GPIO_t* instance, active_low_t* retVal)
{
    if(!ioctl_cmd(instance, GET_ACTIVE_LOW_CMD, retVal))
    {
        printf("Error: ioctl get active low gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}

int GPIO_get_value(GPIO_t* instance, gpio_value_t* retVal)
{
    //Get gpio_value
    //Call ioctl_cmd(), pass the GPIO struct to ioctl_cmd(), pass cmd GET_VALUE_CMD
    //Pass retVal
    //if ioctl_cmd() return SUCCESS, then read the retVal in other place, return SUCCESS
    //else print error log, return ERROR
    if(!ioctl_cmd(instance, GET_VALUE_CMD, retVal))
    {
        printf("Error: ioctl get value gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}
int GPIO_get_export_status(GPIO_t* instance, export_t* retVal)
{
    int dirStat = ioctl_is_exported(instance->gpio_num);
    if((instance->export_status == EXPORTED)
        &&(dirStat == TRUE))
        *retVal = EXPORTED;
    else if((instance->export_status == UNEXPORTED)
            &&(dirStat == FALSE))
        *retVal = UNEXPORTED;
    else{
        *retVal = -1;
        printf("Error: internal core error gpio%d\n", instance->gpio_num);
        return ERROR;
    }
    return SUCCESS;
}
int GPIO_get_direction(GPIO_t* instance, direction_t* retVal)
{
    if(!ioctl_cmd(instance, GET_DIR_CMD, retVal))
    {
        printf("Error: ioctl cmd get direction gpio%d\n", instance->gpio_num);
        return ERROR;
    }

    return SUCCESS;
}

int ioctl_cmd(GPIO_t* instance, gpio_command_t cmd, void* val)
{
    //Perform GPIO check before implement on hardware
    //  1)EXPORT_CMD: Check instance.export_status == UNEXPORTED ?
    //                if yes {check is_exported() if return TRUE change export_status -> EXPORTED, then return SUCCESS
    //                                            else call ioctl_cmd_export(). If return SUCCESS,change export_status -> EXPORTED, return SUCCESS, else return ERROR}
    //                else {check is_exported() if return TRUE just return SUCCESS and print log "already exported\n"}
    //                                          else call ioctl_cmd_export(). If return SUCCESS, return SUCCESS, else change export_status -> UNEXPORTED, return ERROR}
    //  2)UNEXPORTED_CMD: Check instance.export_status == EXPORTED ?
    //                if yes {check is_exported() if return FALSE change export_status -> UNEXPORTED, then return SUCCESS
    //                                            else call ioctl_cmd_unexport(). If return SUCCESS,change export_status -> UNEXPORTED, return SUCCESS, else return ERROR}
    //                else {check is_exported() if return FALSE just return SUCCESS and print log "already unexported\n"}
    //                                          else call ioctl_cmd_unexport(). If return SUCCESS, return SUCCESS, else change export_status -> UNEXPORTED, return ERROR}
    //  3)SET_DIR_CMD:          check is_exported() && export_status == EXPORTED ?
    //                          if yes call ioctl_cmd_set_dir(), if return SUCCESS, return SUCCESS, else return ERROR
    //                          else print log "please export first\n", return ERROR
    //  4)SET_ACTIVE_LOW_CMD:   check is_exported() && export_status == EXPORTED ?
    //                          if yes call ioctl_cmd_set_active_low(), if return SUCCESS, return SUCCESS, else return ERROR
    //                          else print log "please export first\n", return ERROR
    //  5)SET_VALUE_CMD:        check is_exported() && export_status == EXPORTED ?
    //                          if yes call ioctl_cmd_set_value, if return SUCCESS, return SUCCESS, else return ERROR          
    //                          else print log "please export first\n", return ERROR
    //  6)GET_DIR_CMD:
    //  7)GET_ACTIVE_LOW_CMD:
    //  8)GET_VALUE_CMD:        check is_exported() && export_status == EXPORTED ?
    //                          if yes call ioctl_cmd_get_value, if return SUCCESS, return SUCCESS, else return ERROR          
    //                          else print log "please export first\n", return ERROR

    if(cmd == EXPORT_CMD || cmd == UNEXPORT_CMD)
    {
        int dir_exist = ioctl_is_exported(instance->gpio_num);
        //printf("%d\n", dir_exist);
        if(cmd == EXPORT_CMD)
        {
            if(instance->export_status == UNEXPORTED)
            {
                if(dir_exist == TRUE)
                {
                    instance->export_status = EXPORTED;
                    return SUCCESS;
                }
                else{
                    //printf("here\n");
                    if(!ioctl_cmd_export(instance->gpio_num))
                        return ERROR;
                    //printf("success\n");
                    instance->export_status = EXPORTED;
                    return SUCCESS;
                }
            }
            else if(instance->export_status == EXPORTED)
            {
                if(dir_exist == TRUE)
                {
                    printf("already exported\n");
                    return SUCCESS;
                }
                else{
                    if(!ioctl_cmd_export(instance->gpio_num))
                    {
                        instance->export_status = UNEXPORTED;
                        return ERROR;
                    }
                    return SUCCESS;
                }
            }
            else{
                //do nothing
            }
        }
        else
        {
            if(instance->export_status == EXPORTED)
            {
                if(dir_exist == FALSE)
                {
                    instance->export_status = UNEXPORTED;
                    return SUCCESS;
                }
                else{
                    if(!ioctl_cmd_unexport(instance->gpio_num))
                        return ERROR;
                    instance->export_status = UNEXPORTED;
                    return SUCCESS;
                }
            }
            else if(instance->export_status == UNEXPORTED)
            {
                if(dir_exist == FALSE)
                {
                    printf("already unexported\n");
                    return SUCCESS;
                }
                else{
                    if(!ioctl_cmd_export(instance->gpio_num))
                    {
                        instance->export_status = EXPORTED;
                        return ERROR;
                    }
                    return SUCCESS;
                }
            }
            else{
                //do nothing
            }
        }
            
    }
    else
    {
        export_t    tmpExport;
        direction_t tmp;
        int tmpNum;
        int dir_exist = ioctl_is_exported(instance->gpio_num);
        if(dir_exist == TRUE)
            tmpExport = EXPORTED;
        else
            tmpExport = UNEXPORTED;
        
        if(instance->export_status != tmpExport)
        {
            printf("Error: internal error export status gpio%d\n", instance->gpio_num);
            return ERROR;
        }
        int ret;
        switch (cmd)
        {
        case SET_DIR_CMD:
            ret = (ioctl_cmd_set_dir(instance->gpio_num, *((direction_t*)val) )) ? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                instance->direction = *((direction_t*)val);
            } 
            break;
        case SET_ACTIVE_LOW_CMD:
            ret = (ioctl_cmd_set_active_low(instance->gpio_num, *((active_low_t*)val)))? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                instance->active_low = *((active_low_t*)val);
            } 
            break;
        case SET_VALUE_CMD:
            tmpNum = instance->gpio_num;
            tmpNum += (int)(instance->direction) << 16;
            if(!ioctl_cmd_get_dir(tmpNum, &tmp))
            {
                printf("Error: Cannot detect input/output\n");
                ret = ERROR;
                break;
            }
            if(tmp == INPUT)
            {
                ret = SUCCESS;
                break;
            }
            ret = (ioctl_cmd_set_value(instance->gpio_num, *((gpio_value_t*)val)))? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                instance->value = *((gpio_value_t*)val);
            }
            break;
        case SET_EDGE_CMD:
            ret = (ioctl_cmd_set_edge(instance->gpio_num, *((edge_t*)val)))? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                //printf("here set\n");
                instance->edge = *((edge_t*)val);
            }
            break;
        case GET_DIR_CMD:
            tmpNum = instance->gpio_num;
            tmpNum += (int)(instance->direction) << 16;
            ret = (ioctl_cmd_get_dir(tmpNum, (direction_t*)val))? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                if(instance->direction != *(direction_t*)val)
                    ret = ERROR;
            }
            break;
        case GET_ACTIVE_LOW_CMD:
            ret = (ioctl_cmd_get_active_low(instance->gpio_num, (active_low_t*)val)) ? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                if(instance->active_low != *(active_low_t*)val)
                    ret = ERROR;
            }
            break;
        case GET_VALUE_CMD:
            ret = (ioctl_cmd_get_value(instance->gpio_num, (gpio_value_t*)val)) ? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                if(instance->value != *(gpio_value_t*)val);
                ret = ERROR;
            }
            break;
        case GET_EDGE_CMD:
            tmpNum = instance->gpio_num;
            //printf("ioclt edge=%d\n", instance->edge);
            tmpNum += (int)(instance->edge) << 16;
            ret = (ioctl_cmd_get_edge(tmpNum, (edge_t*)val))? SUCCESS : ERROR;
            if(ret == SUCCESS)
            {
                //printf("here get\n");
                if(instance->edge != *(edge_t*)val);
                ret = ERROR;
            }
            break;
        default:
            break;
        }
        if(ret == ERROR)
            val = (void*)-1;
        return ret;   
    }
    return SUCCESS;
}
int ioctl_is_exported(int num)
{
    //check check in /sys/class/gpio/ are there /gpio_num or not ?
    //return TRUE if yes
    //else return FALSE
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    struct stat sb;
    if(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//All below function directly run on file:
//ie: /sys/class/gpio/
/*set functions run write():1)set_active_low: /gpio_num/active_low
                            2)export:   /export
                            3)unexport: /unexport
                            4)set_dir:  /gpio_num/direction
                            5)set_value:/gpio_num/value                              
*/
//get functions run read(): get_value:  /gpio_num/value
int ioctl_cmd_set_active_low(int num, active_low_t val)
{
    int fd;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, ACTIVE_LOW);
    char str_val[2];
    sprintf(str_val, "%d" , (int)val);
    //itoa()
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd set active low open file\n");
        return ERROR;
    }
    if(write(fd, str_val, 1) != 1)
    {
        printf("Error : ioctl set active low write file\n");
        return ERROR;
    }
    close(fd);
    return SUCCESS;
}
int ioctl_cmd_export(int num)
{
    int fd, size;
    char path[MAX_PATH_SIZE]={0}, number_char[MAX_PIN_SIZE] = {0};
    strcat(path, GPIO_PATH);
    if(num > 9) size = 2;
    else size = 1;
    sprintf(number_char, "%d", num);
    strcat(path, EXPORT);
    //printf("%s\n", path);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd export open file\n");
        return ERROR;
    }
    if(write(fd, number_char, size) != size)
    {
        printf("Error : ioctl cmd export write file\n");
        return ERROR;
    }
    close(fd);
    return SUCCESS;
}
int ioctl_cmd_unexport(int num)
{
    int fd, size;
    char path[MAX_PATH_SIZE]={0}, number_char[MAX_PIN_SIZE] = {0};
    strcat(path, GPIO_PATH);
    if(num > 9) size = 2;
    else size = 1;
    sprintf(number_char, "%d", num);
    strcat(path, UNEXPORT);
    //printf("%s\n", path);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd unexport open file\n");
        return ERROR;
    }
    if(write(fd, number_char, size) != size)
    {
        printf("Error : ioctl set unexport write file\n");
        return ERROR;
    }
    close(fd);
    return SUCCESS;
}
int ioctl_cmd_set_dir(int num, direction_t dir)
{
    int fd, size;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, DIRECTION);
    //printf("%s\n", path);
    char str_val[4] = {0};
    //char *str_val;
    if(dir == OUTPUT)
    {
        size = 4;
        //str_val = (char*)malloc((size+1)*sizeof(char));
        strcat(str_val, "out");
    }
    else if(dir == INPUT)
    {
        size = 3;
        //str_val = (char*)malloc((size+1)*sizeof(char));
        strcat(str_val, "in");
    }
    else
    {
        printf("Invalid type\n");
        return ERROR;
    }
    
        
    //fprintf(str_val, "%d", (int)val);
    //printf("set dir: %s\n", str_val);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd set dir open file\n");
        return ERROR;
    }
    if(write(fd, str_val, size-1) != size-1)
    {
        printf("Error : ioctl cmd set dir write file\n");
        return ERROR;
    }
    close(fd);
    return SUCCESS;
}

int ioctl_cmd_set_edge(int num, edge_t ed)
{
int fd, size;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, EDGE);
    //printf("%s\n", path);
    //char* str_val;
    char str_val[8] = {0};
    if(ed == NONE)
    {
        size = 5;
        //str_val = (char*)malloc((size+1)*sizeof(char));
        strcat(str_val, "none");
    }
    else if(ed == RISING)
    {
        size = 7;
        //str_val = (char*)malloc((size+1)*sizeof(char));
        strcat(str_val, "rising");
    }
    else if(ed == FALLING)
    {
        size = 8;
        //str_val = (char*)malloc((size+1)*sizeof(char));
        strcat(str_val, "falling");
    }
    else if(ed == BOTH)
    {
        size = 5;
        //str_val = (char*)malloc((size+1)*sizeof(char));
        strcat(str_val, "both");
    }
    else
    {
        printf("Invalid type\n");
        return ERROR;
    }

    
    //fprintf(str_val, "%d", (int)val);
    //printf("set edge: %s\n", str_val);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd set edge open file\n");
        return ERROR;
    }
    if(write(fd, str_val, size-1) != size-1)
    {
        printf("Error : ioctl cmd set edge write file\n");
        return ERROR;
    }
    close(fd);
    return SUCCESS;
}

int ioctl_cmd_get_edge(int num, edge_t* val)
{
    int gpionum = num & LOW_HALF;
    int edge = (num & HIGH_HALF) >> 16;

    //printf("gpionum=%d\n", gpionum);
    //printf("edge=%d\n", edge);
    int size=0;
    if(edge == FALLING)
        size = 8;
    else if(edge == RISING)
        size = 7;
    else if(edge == NONE || edge == BOTH)
        size = 5;
    int fd;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", gpionum);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, EDGE);
        //printf("%s\n", path);
    char str_val[size];

    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd get edge open file\n");
        return ERROR;
    }
    if(read(fd, str_val, size-1) != size-1)
    {
        printf("Error : ioctl cmd get edge read file\n");
        return ERROR;
    }
    str_val[size-1] = '\0';
    //printf("%s\n", str_val);
    if(strcmp(str_val, "falling") == 0)
    {
        *val = FALLING;
    }
    else if(strcmp(str_val, "rising") == 0)
    {
        //printf("here\n");
        *val = RISING;
    }
    else if(strcmp(str_val, "none") == 0)
    {
        *val = NONE;
    }
    else if(strcmp(str_val, "both") == 0)
    {
        *val = BOTH;
    }
    else{
        //do nothing
    }
    close(fd);
    return SUCCESS;
}

int ioctl_cmd_set_value(int num, gpio_value_t val)
{
    int fd;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, VALUE);
    //printf("%s\n", path);
    char str_val[2];
    sprintf(str_val, "%d", (int)val);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd set value open file\n");
        return ERROR;
    }
    if(write(fd, str_val, 1) != 1)
    {
        printf("Error : ioctl cmd set value write file\n");
        return ERROR;
    }
    close(fd);
    return SUCCESS;
}
int ioctl_cmd_get_value(int num, gpio_value_t* val)
{
    int fd;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, VALUE);
        //printf("%s\n", path);
    char str_val;
    // fprintf(str_val, "%d", (int)val);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    lseek(fd, 0, SEEK_SET);
    if(fd == -1)
    {
        printf("Error: ioctl cmd get value open file\n");
        return ERROR;
    }
    if(read(fd, &str_val, 1) != 1)
    {
        printf("Error : ioctl cmd get value write file\n");
        return ERROR;
    }
    int tmp = atoi(&str_val);
    *val = (gpio_value_t)tmp;
    close(fd);
    return SUCCESS;
}

int ioctl_cmd_get_dir(int num, direction_t* val)
{
    int gpionum = num & LOW_HALF;
    //printf("gpionum=%d\n", gpionum);

    int dir = (num & HIGH_HALF) >> 16;
    //printf("dir=%d\n", dir);
    int size= 0;
    if(dir == OUTPUT)
        size = 4;
    else if(dir == INPUT)
        size = 3;
    
    int fd;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", gpionum);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, DIRECTION);
        //printf("%s\n", path);
    char str_val[size];
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd get dir open file\n");
        return ERROR;
    }
    if(read(fd, str_val, size-1) != size-1)
    {
        printf("Error : ioctl cmd get dir write file\n");
        return ERROR;
    }
    str_val[size-1] = '\0';
    //printf("%s\n", str_val);
    if(strcmp(str_val, "out") == 0)
    {
        *val = OUTPUT;
    }
    else if(strcmp(str_val, "in") == 0)
    {
        *val = INPUT;
    }
    close(fd);
    return SUCCESS;
}
int ioctl_cmd_get_active_low(int num, active_low_t* val)
{
    int fd;
    char name[MAX_NAME_SIZE] = {0}, number_char[MAX_PIN_SIZE] ={0};
    strcat(name, GPIO_PREFIX);
    sprintf(number_char, "%d", num);
    strcat(name, number_char);
    char path[MAX_PATH_SIZE]={0};
    strcat(path, GPIO_PATH);
    strcat(path, name);
    strcat(path, ACTIVE_LOW);
        //printf("%s\n", path);
    char str_val;
    // fprintf(str_val, "%d", (int)val);
    fd = open(path, FILE_FLAGS, FILE_PERMS);
    if(fd == -1)
    {
        printf("Error: ioctl cmd get active low open file\n");
        return ERROR;
    }
    if(read(fd, &str_val, 1) != 1)
    {
        printf("Error : ioctl get active low write file\n");
        return ERROR;
    }
    int tmp = atoi(&str_val);
    *val = (active_low_t)tmp;
    close(fd);
    return SUCCESS;
}