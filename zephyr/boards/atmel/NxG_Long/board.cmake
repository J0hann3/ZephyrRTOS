board_set_debugger_ifnset(openocd)
board_set_flasher_ifnset(openocd)

board_runner_args(jlink "--device=ATSAMD21J18A")

include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
include(${ZEPHYR_BASE}/boards/common/pyocd.board.cmake)
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)