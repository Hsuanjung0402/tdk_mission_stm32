uint8 PHASE_IDLE = 0
uint8 PHASE_PICKING = 1
uint8 PHASE_ERROR = 2

uint8 phase           # PHASE_*
uint8 pick_count      # 開機以來夾取完成的累計數（只增不減，uint8 溢位沒關係）
bool armed            # 目前是否允許「碰到開關就自動夾取」
bool switch_pressed   # 前方極限開關原始狀態
