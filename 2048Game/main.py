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
FONT = ("Verdana", 38, "bold")
SCORE_FONT = ("Verdana", 24, "bold")
# Keys used by TkInter
KEY_UP = "'w'"
KEY_RIGHT = "'d'"
KEY_DOWN = "'s'"
KEY_LEFT = "'a'"
KEY_REVERT = "'z'"
KEY_CLEAR = "' '"
# Frames and components of TkInter GUI
main_frame = Frame()
background_frame = None
score_label = None
grid = []
# Main components of the game
matrix = []  # Contains numbers of all cells
GAME_STATE_OVER = -1
GAME_STATE_OK = 0
GAME_STATE_WIN = 1


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
def place_cell():
    i, j = get_free_indexes()
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
    for i in range(len(matrix_in)):
        for j in range(len(matrix_in[i]) - 1):
            if matrix_in[i][j] != 0 and matrix_in[i][j] == matrix_in[i][j + 1]:
                matrix_in[i][j] *= 2
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


# Dictionary contains all actions
action_handlers = {
    KEY_UP: up_action_handler,
    KEY_RIGHT: right_action_handler,
    KEY_DOWN: down_action_handler,
    KEY_LEFT: left_action_handler
}


# Since we done with handlers, we need to attach them to main_frame
# Main handler for main_frame
def action_handler(event):
    key = repr(event.char)
    if key in action_handlers:
        success = action_handlers[key]()
        if success:
            state = get_game_state()
            place_cell()
            update_grid()
            if state == GAME_STATE_OVER:
                set_center_text("You", "lose")
            elif state == GAME_STATE_WIN:
                set_center_text("You", "win!")


main_frame.master.bind("<Key>", action_handler)
main_frame.master.title("2048 Game")


# Returns GAME_STATE
def get_game_state():
    global matrix
    # If one of the cells is 2048 - it's WIN
    for row in matrix:
        for value in row:
            if value == 2048:
                return GAME_STATE_WIN
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
                         fg=FOREGROUND_COLOR_DICT[2048],
                         text=left_cell_text)
    grid[1][2].configure(bg=BACKGROUND_COLOR_CELL_EMPTY,
                         fg=FOREGROUND_COLOR_DICT[2048],
                         text=right_cell_text)


# Initialize all components
def init():
    init_matrix()
    init_grid()
    place_cell()  # Two starting cells
    place_cell()
    update_grid()
    mainloop()


if __name__ == '__main__':
    init()
