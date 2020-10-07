import random
from tkinter import *
# Constants and TkInter GUI variables
WINDOW_SIZE = 400  # Window width and height
GRID_SIZE = 4  # Grid size 4x4
GRID_PADDING = 10
BACKGROUND_COLOR_GAME = "#92877d"  # Used as a default background color
BACKGROUND_COLOR_CELL_EMPTY = "#9e948a"  # Used for empty cell color
BACKGROUND_COLOR_CELL_EXCESSIVE = "#ff3728"  # All cells above 2048 will get this color
BACKGROUND_COLOR_DICT = {  # Background color of a cell. For values above, use BACKGROUND_COLOR_CELL_EXCESSIVE
    2: "#eee4da",
    4: "#ede0c8",
    8: "#f2b179",
    16: "#f59563",
    32: "#f67c5f",
    64: "#f65e3b",
    128: "#edcf72",
    256: "#edcc61",
    512: "#edc850",
    1024: "#edc53f",
    2048: "#edc22e",
}
FOREGROUND_COLOR_DICT = {  # Text color of a cell
    2: "#776e65",
    4: "#776e65",
    8: "#f9f6f2",
    16: "#f9f6f2",
    32: "#f9f6f2",
    64: "#f9f6f2",
    128: "#f9f6f2",
    256: "#f9f6f2",
    512: "#f9f6f2",
    1024: "#f9f6f2",
    2048: "#f9f6f2"
}
DEFAULT_TEXT_COLOR = "#f9f6f2"
FONT = ("Verdana", 38, "bold")
SCORE_FONT = ("Verdana", 24, "bold")
# Keys used by TkInter
KEY_UP = "'w'"
KEY_RIGHT = "'d'"
KEY_DOWN = "'s'"
KEY_LEFT = "'a'"
KEY_REVERT = "'z'"  # extension: revert
KEY_RESET = "' '"  # extension: reset
# Frames and components of TkInter GUI
main_frame = Frame()
background_frame = None
score_label = None  # extension: score
score_frame = None  # extension: score
grid = []
# Main components of the game
matrix = []  # Contains numbers of all cells
GAME_STATE_OVER = -1
GAME_STATE_OK = 0
GAME_STATE_WIN = 1
score = 0  # extension: score
reverted = False  # extension: revert
prev_matrix = []  # extension: revert
prev_score = 0  # extension: revert + score
cleared = False  # extension: reset; to prevent unnecessary clearings
current_game_state = GAME_STATE_OK  # extension: endless mode


# Initialize matrix with zeros
def init_matrix():
    global matrix
    for _ in range(GRID_SIZE):
        matrix.append([0] * GRID_SIZE)


# Return free position in matrix depending on GRID_SIZE
def get_free_indexes():
    i, j = random.randint(0, GRID_SIZE - 1), random.randint(0, GRID_SIZE - 1)
    while matrix[i][j] != 0:
        i = random.randint(0, GRID_SIZE - 1)
        j = random.randint(0, GRID_SIZE - 1)
    return i, j


# Place a cell containing 2
# extension: with some probability it can be 4 instead of 2
def place_cell():
    i, j = get_free_indexes()
    prob = random.randint(1, 100) / 100
    if prob > 0.9:
        matrix[i][j] = 4
    else:
        matrix[i][j] = 2


# Initialize grid with empty cells
def init_grid():
    global background_frame
    global grid
    background_frame = Frame(bg=BACKGROUND_COLOR_GAME,
                             width=WINDOW_SIZE,
                             height=WINDOW_SIZE)
    background_frame.grid()
    for i in range(GRID_SIZE):
        grid_row = []
        for j in range(GRID_SIZE):
            cell = Frame(master=background_frame,
                         bg=BACKGROUND_COLOR_CELL_EMPTY,
                         width=WINDOW_SIZE / GRID_SIZE,
                         height=WINDOW_SIZE / GRID_SIZE)
            cell.grid(row=i,
                      column=j,
                      padx=GRID_PADDING,
                      pady=GRID_PADDING)
            label = Label(master=cell,
                          bg=BACKGROUND_COLOR_CELL_EMPTY,
                          text="",
                          font=FONT,
                          justify=CENTER,
                          width=5,
                          height=2
                          )
            label.grid()
            grid_row.append(label)
        grid.append(grid_row)


# Update grid to render current values inside matrix
def update_grid():
    global matrix
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if matrix[i][j] == 0:
                grid[i][j].configure(bg=BACKGROUND_COLOR_CELL_EMPTY,
                                     text="")
            elif matrix[i][j] > 2048:
                grid[i][j].configure(bg=BACKGROUND_COLOR_CELL_EXCESSIVE,
                                     fg=DEFAULT_TEXT_COLOR,
                                     text=str(matrix[i][j]))
            else:
                grid[i][j].configure(bg=BACKGROUND_COLOR_DICT[matrix[i][j]],
                                     fg=FOREGROUND_COLOR_DICT[matrix[i][j]],
                                     text=str(matrix[i][j]))


# Shifts all values of a matrix to the left side without merging them
# Returns new shifted matrix and boolean that indicates that there was at least one shift
# In general it's used to prevent unnecessary actions in future
def shift_matrix(matrix_in):
    shifted = False
    result_matrix = []
    for i in range(len(matrix_in)):
        result_matrix.append([0] * len(matrix_in))
        shift_pos = 0
        for j in range(len(matrix_in[i])):
            if matrix_in[i][j] != 0:
                result_matrix[i][shift_pos] = matrix_in[i][j]
                if shift_pos != j:  # If shift_pos is not equals to j, it means the row hasn't changed
                    shifted = True
                shift_pos += 1
    return result_matrix, shifted


# Merge all cells of matrix_in to the left direction
# Returns merged matrix and bool that indicates that there was at least one merging
def merge_matrix(matrix_in):
    merged = False
    global score  # extension: score
    for i in range(len(matrix_in)):
        for j in range(len(matrix_in[i]) - 1):
            if matrix_in[i][j] != 0 and matrix_in[i][j] == matrix_in[i][j + 1]:
                matrix_in[i][j] *= 2
                score += matrix_in[i][j]  # extension: score
                matrix_in[i][j + 1] = 0
                merged = True
    return matrix_in, merged


# Mirrors a matrix and returns new matrix
# Example:
# 1 2 3 4      4 3 2 1
# 5 6 7 8  ->  8 7 6 5
# 9 a b c      c b a 9
def mirror_matrix(matrix_in):
    mirrored_matrix = []
    for i in range(len(matrix_in)):
        mirrored_matrix.append([])
        for j in range(len(matrix_in[i])):
            mirrored_matrix[i].append(matrix_in[i][len(matrix_in) - j - 1])
    return mirrored_matrix


# Transposing a matrix. All [i][j] values will become [j][i]
# Return transposed matrix
def transpose_matrix(matrix_in):
    transposed_matrix = []
    for i in range(len(matrix_in)):
        transposed_matrix.append([])
        for j in range(len(matrix_in[i])):
            transposed_matrix[i].append(matrix_in[j][i])
    return transposed_matrix


# Event handler for moving and merging all cells to the left direction
# Returns boolean that indicates success of either merging or shifting cells
def left_action_handler():
    global matrix
    temp_matrix, shift_success = shift_matrix(matrix)  # We will simply shift matrix
    temp_matrix, merge_success = merge_matrix(temp_matrix)  # And then merge all cells
    matrix = shift_matrix(temp_matrix)[0]  # It's necessary to make shift again to move all cells again to the left
    return shift_success or merge_success


# Event handler for moving and merging all cells to the right direction
# We need to mirror our original matrix to perform left side shift and merge
def right_action_handler():
    global matrix
    temp_matrix, shift_success = shift_matrix(mirror_matrix(matrix))  # Before we start, we must reverse matrix
    temp_matrix, merge_success = merge_matrix(temp_matrix)  # Since matrix is mirrored, we can merge to the left side
    temp_matrix = shift_matrix(temp_matrix)[0]
    matrix = mirror_matrix(temp_matrix)  # Since we done, me must return matrix to its original state
    return shift_success or merge_success


# Event handler for moving and merging all cells to the upper direction
# This action needs transposing matrix since we need to deal with upper cells
def up_action_handler():
    global matrix
    temp_matrix, shift_success = shift_matrix(transpose_matrix(matrix))
    temp_matrix, merge_success = merge_matrix(temp_matrix)
    temp_matrix = shift_matrix(temp_matrix)[0]
    matrix = transpose_matrix(temp_matrix)
    return shift_success or merge_success


# Event handler for moving and merging all cells to the lower direction
# In this case we need both transposing and mirroring
def down_action_handler():
    global matrix
    temp_matrix, shift_success = shift_matrix(mirror_matrix(transpose_matrix(matrix)))
    temp_matrix, merge_success = merge_matrix(temp_matrix)
    temp_matrix = shift_matrix(temp_matrix)[0]
    matrix = transpose_matrix(mirror_matrix(temp_matrix))
    return shift_success or merge_success


# extension: revert
# Event handler for reverting one action
def revert_action_handler():
    global score
    global prev_score
    global reverted
    global cleared
    if reverted:  # Only one revert can be happen at time
        return
    global matrix
    global prev_matrix
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            matrix[i][j] = prev_matrix[i][j]
    reverted = True  # Set a flag to prevent unnecessary revert actions
    score = prev_score
    update_scoreboard()
    update_grid()
    cleared = False  # extension: reset; it also will affect resetting


# extension: reset
# Clears game field and restarts the game
def clear_action_handler():
    global cleared
    if cleared:
        return
    global score
    global reverted
    global current_game_state
    # We will not reset prev_score, because it could be useful
    reset_matrix()
    score = 0
    update_scoreboard()
    place_cell()
    place_cell()
    update_grid()
    reverted = False
    cleared = True
    current_game_state = GAME_STATE_OK  # extension: endless mode


# Since we done with handlers, we need to attach them to main_frame
# Main handler for main_frame
def action_handler(event):
    global reverted  # extension: revert
    global score  # extension: revert
    global prev_score  # extension: revert
    global cleared  # extension: reset
    global current_game_state  # extension: endless mode
    key = repr(event.char)
    # extension: revert
    # we just don't want other actions
    if key == KEY_REVERT or key == KEY_RESET:
        action_handlers[key]()
        return

    if key in action_handlers and current_game_state != GAME_STATE_OVER:
        prev_score = score
        save_current_matrix()
        success = action_handlers[key]()
        game_state = get_game_state()
        if success:
            place_cell()
            update_grid()
            update_scoreboard()  # extension: score
            reverted = False  # extension: revert
            cleared = False  # extension: reset
        if game_state == GAME_STATE_OVER:
            set_center_text("You", "lose")
            current_game_state = GAME_STATE_OVER
            reverted = True  # extension: endless more; prevent unnecessary revert when game is over


# Dictionary contains all actions
action_handlers = {
    KEY_UP: up_action_handler,
    KEY_RIGHT: right_action_handler,
    KEY_DOWN: down_action_handler,
    KEY_LEFT: left_action_handler,
    KEY_REVERT: revert_action_handler,
    KEY_RESET: clear_action_handler
}
main_frame.master.bind("<Key>", action_handler)
main_frame.master.title("2048 Game")


# Returns the current state of the game
def get_game_state():
    global matrix
    # extension: endless mode; removed unnecessary check
    # # If one of the cells is 2048 - it's WIN
    # for row in matrix:
    #     for value in row:
    #         if value == 2048:
    #             return GAME_STATE_WIN
    # If at least one cell is a zero - it's OK
    for row in matrix:
        for value in row:
            if value == 0:
                return GAME_STATE_OK
    # If neighbour cell can be merged with the current cell - it's OK
    # This loop does not perform lower boundary checks
    for i in range(len(matrix) - 1):
        for j in range(len(matrix[i]) - 1):
            if matrix[i][j] == matrix[i][j + 1] or matrix[i][j] == matrix[i + 1][j]:
                return GAME_STATE_OK
    # This loop performs lower boundary checks by row and column
    # 1,  2,  3,  4*
    # 5,  6,  7,  8*
    # 8,  a,  b,  c*
    # d*, e*, f*, g*
    for x in range(len(matrix) - 1):
        if matrix[len(matrix) - 1][x] == matrix[len(matrix) - 1][x + 1] or \
           matrix[x][len(matrix) - 1] == matrix[x + 1][len(matrix) - 1]:
            return GAME_STATE_OK
    # Otherwise the game is over
    return GAME_STATE_OVER


# Utility function to show text
def set_center_text(left_cell_text, right_cell_text):
    global grid
    grid[1][1].configure(bg=BACKGROUND_COLOR_CELL_EMPTY,
                         fg=DEFAULT_TEXT_COLOR,
                         text=left_cell_text)
    grid[1][2].configure(bg=BACKGROUND_COLOR_CELL_EMPTY,
                         fg=DEFAULT_TEXT_COLOR,
                         text=right_cell_text)


# extension: score
# Initializes special block under game field
def init_scoreboard():
    global score_label
    global score_frame
    score_frame = Frame(master=background_frame,
                        bg=BACKGROUND_COLOR_GAME,
                        width=WINDOW_SIZE,
                        height=32)
    score_frame.grid(row=4,
                     column=0,
                     columnspan=4)
    score_label = Label(master=score_frame,
                        text=str(0),
                        font=SCORE_FONT,
                        bg=BACKGROUND_COLOR_GAME,
                        fg=DEFAULT_TEXT_COLOR)
    score_label.grid(pady=GRID_PADDING * 2)


# extension: score
# Updates value in scoreboard
def update_scoreboard():
    global score_label
    if score_label is not None:
        score_label.configure(text=str(score))


# extension: revert
# Initializes prev_matrix with default values
# It will be used to revert one change back
def init_prev_matrix():
    for _ in range(GRID_SIZE):
        prev_matrix.append([0] * GRID_SIZE)


# extension: revert
# Creates a copy of current matrix to prev_matrix
def save_current_matrix():
    global matrix
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            prev_matrix[i][j] = matrix[i][j]


# extension: reset
# Resets current matrix. Filling all values with zeros
def reset_matrix():
    global matrix
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            matrix[i][j] = 0


# Initialize all components
def init():
    init_matrix()
    init_prev_matrix()
    init_grid()
    init_scoreboard()  # extension: score
    place_cell()  # Two starting cells
    place_cell()
    update_grid()
    mainloop()


if __name__ == '__main__':
    init()
