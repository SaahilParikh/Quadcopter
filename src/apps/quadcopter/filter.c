#define resolution 100
typedef struct{
    float filter[resolution];
    int location;
    float value;
} average_filter;

void init_average_filter(average_filter *filter){
    for (int i = 0; i < resolution; i++) {
        filter->filter[i] = 0.0;
    }
    filter->location = 0;
    filter->value = 0;
}

void add_average_filter(average_filter *filter, float value){
    float prev_value = filter->filter[filter->location];
    filter->filter[filter->location] = value;
    filter->value += ((value - prev_value)/resolution);
    filter->location = (filter->location + 1)%resolution;
}

float get_average_filter(average_filter *filter) {
    return filter->value;
}