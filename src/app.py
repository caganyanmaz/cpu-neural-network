import ctypes
import pygame

# API Functions:
# Network *api_load_network_from_file(const char *filename)
# void api_destroy_network(Network *network)
# int api_get_result(Network *network, char values[IMAGE_HEIGHT][IMAGE_WIDTH])

IMAGE_HEIGHT    = 28
IMAGE_WIDTH     = 28
CELL_SIZE       = 30
FOOTER_HEIGHT   = 100
PRIMARY_COLOR   = (0, 0, 0)
SECONDARY_COLOR = (70, 80, 130)

def main():
    api = ctypes.CDLL("./api.so")
    configure_api_functions(api)
    model_name = ctypes.c_char_p(b"../data/default.model")
    network = api.api_load_network_from_file(model_name)

    pygame.font.init()
    screen = pygame.display.set_mode((IMAGE_WIDTH * CELL_SIZE, IMAGE_HEIGHT * CELL_SIZE + FOOTER_HEIGHT)) 
    color_button = {"pos": (100, IMAGE_HEIGHT * CELL_SIZE + 10), "surface": pygame.Surface((300, FOOTER_HEIGHT - 20)), "text": "Switch color", "text_pos": (50, 20) }
    guess_button = {"pos": (500, IMAGE_HEIGHT * CELL_SIZE + 10), "surface": pygame.Surface((300, FOOTER_HEIGHT - 20)), "text": "Let me guess!", "text_pos": (50, 20) }
    cell_color_option = True
    pygame.display.set_caption('Number Guesser') 
    window = pygame.display.get_surface()
    board = create_board()
    board[1][2] = True
    paint_board(window, board)
    paint_footer(window, [color_button, guess_button])
    pygame.display.flip()

    running = True
    print(color_button["surface"].get_rect())

    while running: 
        for event in pygame.event.get(): 
            if event.type == pygame.QUIT: 
                running = False
            if event.type == pygame.MOUSEBUTTONUP:
                print("Mouse up!")
                pos = pygame.mouse.get_pos()
                if is_colliding_with_button(color_button, pos):
                    print("Color button clicked!")
                    cell_color_option = not cell_color_option
                if is_colliding_with_button(guess_button, pos):                    
                    print(guess(board))
            if event.type == pygame.MOUSEBUTTONDOWN:
                print("hi")

        paint_board(window, board)
        paint_footer(window, [color_button])
        pygame.display.flip()

    api.api_destroy_network(network)

def paint_board(window, board):
    for i in range(IMAGE_HEIGHT):
        for j in range(IMAGE_WIDTH):
            pygame.draw.rect(window, SECONDARY_COLOR, pygame.Rect(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE))
            if not board[i][j]:
                pygame.draw.rect(window, PRIMARY_COLOR, pygame.Rect(i * CELL_SIZE + 1, j * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2))
 
def paint_footer(window, buttons):
    font = pygame.font.Font(None, 35)
    for button in buttons:
        text = font.render(button["text"], True, (0, 0, 0))
        button["surface"].fill((255, 255, 255))
        button["surface"].blit(text, button["text_pos"])
        window.blit(button["surface"], button["pos"])

def is_colliding_with_button(button, pos):
    button_pos_x, button_pos_y = button["pos"]
    pos_x, pos_y = pos
    pos_x -= button_pos_x
    pos_y -= button_pos_y
    return button["surface"].get_rect().collidepoint((pos_x, pos_y))


        
def guess(board):
    return "Connect the API"

   

def create_board():
    board = []
    for _ in range(IMAGE_HEIGHT):
        row = []
        for _ in range(IMAGE_WIDTH):
            row.append(False)
        board.append(row)
    return board


def configure_api_functions(api):
    api.api_load_network_from_file.argtypes = [ctypes.c_char_p]
    api.api_load_network_from_file.restype = ctypes.c_void_p
    
    api.api_destroy_network.argtypes = [ctypes.c_char_p]
    api.api_destroy_network.restype  = None

    api.api_get_result.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_ubyte)]
    api.api_get_result.restype  = ctypes.c_int


    
    
if __name__ == "__main__":
    main()
