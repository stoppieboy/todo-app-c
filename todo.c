#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <leif/leif.h>

typedef enum {
    FILTER_ALL = 0,
    FILTER_IN_PROGRESS,
    FILTER_COMPLETED,
    FILTER_LOW,
    FILTER_MEDIUM,
    FILTER_HIGH
} entry_filter;

typedef enum {
    PRIORITY_LOW = 0,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH
} entry_priority;

typedef struct {
    bool completed;
    char* desc, *date;
    entry_priority priority;
} task_entry;

#define WIN_MARGIN 20.0F

static int winw = 1280, winh = 720;
static LfFont titlefont;
static entry_filter current_filter;

static task_entry* entries[1024];
static uint32_t numentries = 0;

static void renderTopBar() {
    lf_push_font(&titlefont);
    lf_text("Your Todo");
    lf_pop_font();

    {
        const float width = 160.0f;

        lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);

        LfUIElementProps props = lf_get_theme().button_props;
        props.margin_left = 0.0f; props.margin_right = 0.0f;
        // props.margin_bottom = 0.0f;
        props.color = (LfColor){65, 167, 204, 255};
        props.border_width = 0.0f; props.corner_radius = 4.0f;

        lf_push_style_props(props);
        lf_set_line_should_overflow(false);
        lf_button_fixed("New task", width, -1);
        lf_set_line_should_overflow(true);
        lf_pop_style_props();
    }
}

static void renderFilters() {
    uint32_t numfilters = 6;
    static const char* filters[] = {"ALL", "IN PROGRESS", "COMPLETED", "LOW", "MEDIUM", "HIGH"};

    LfUIElementProps props = lf_get_theme().button_props;
    props.color = LF_NO_COLOR;
    props.margin_top = 10.0f;
    props.margin_right = 10.0f;
    props.margin_top = 5.0f;
    props.padding = 12.0f;
    props.border_width = 0.0f;
    props.text_color = LF_WHITE;
    props.corner_radius = 8.0f;
    lf_push_style_props(props);

    float width = 0.0f;
    float ptrx_before = lf_get_ptr_x();
    float ptry_before = lf_get_ptr_y();

    lf_set_ptr_y_absolute(lf_get_ptr_y() + 50.0f);

    lf_set_no_render(true);
    for(uint32_t i = 0; i < numfilters; i++){
        lf_button(filters[i]);
    }
    lf_set_no_render(false);

    lf_set_ptr_y_absolute(ptry_before);

    width = lf_get_ptr_x() - ptrx_before - props.margin_right - props.padding;

    lf_set_ptr_x_absolute(winw - width - WIN_MARGIN);

    lf_set_line_should_overflow(false);
    for(uint32_t i = 0; i < numfilters; i++){
        props.color = (current_filter == (entry_filter)i) ? (LfColor){255, 255, 255, 50} : LF_NO_COLOR;
        lf_push_style_props(props);
        if(lf_button(filters[i]) == LF_CLICKED){
            current_filter = (entry_filter)i;
        }
        lf_pop_style_props();
    }
    lf_set_line_should_overflow(true);
}

int main(){
    glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(winw, winh, "Hello World", NULL, NULL);

    glfwMakeContextCurrent(window);

    lf_init_glfw(winw, winh, window);
    
    LfTheme theme = lf_get_theme();
    theme.div_props.color = LF_NO_COLOR;
    lf_set_theme(theme);

    titlefont = lf_load_font("./fonts/inter-bold.ttf", 40);

    task_entry* entry = (task_entry*)malloc(sizeof(*entry));
    entry->priority = PRIORITY_MEDIUM;
    entry->completed = false;
    entry->date = "nothing";
    entry->desc = "Buy a Nimbooz";
    entries[numentries++] = entry;

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        lf_begin();

        lf_div_begin(((vec2s){WIN_MARGIN, WIN_MARGIN}),
                     ((vec2s){winw - WIN_MARGIN * 2.0f, winh - WIN_MARGIN * 2.0f}),
                     true);

        renderTopBar();
        lf_next_line();
        renderFilters();
        lf_next_line();
        {
            lf_div_begin(((vec2s){lf_get_ptr_x(), lf_get_ptr_y()}),
                            ((vec2s){winw - lf_get_ptr_x() - WIN_MARGIN, winh - lf_get_ptr_y() - WIN_MARGIN}),
                            true);
            for(uint32_t i = 0; i < numentries; i++){
                task_entry* entry = entries[i];
                float priority_size = 15.0f;
                // float ptry_before = lf_get_ptr_y();
                lf_set_ptr_x_absolute(lf_get_ptr_x() + 5.0f);
                lf_set_ptr_y_absolute(lf_get_ptr_y() + 5.0f);
                switch (entry->priority){
                    case PRIORITY_LOW:
                        lf_rect(priority_size, priority_size, (LfColor){76, 175, 80, 255}, 4.0f);
                        break;
                    case PRIORITY_MEDIUM:
                        lf_rect(priority_size, priority_size, (LfColor){255, 235, 59, 255}, 4.0f);
                        break;
                    case PRIORITY_HIGH:
                        lf_rect(priority_size, priority_size, (LfColor){244, 67, 54, 255}, 4.0f);
                        break;
                    
                    default:
                        break;
                }
                // lf_set_ptr_y_absolute(ptry_before);
                lf_text(entry->desc);
                lf_next_line();
            }
            lf_div_end();
        }

        lf_div_end();
        lf_end();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    lf_free_font(&titlefont);
    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}