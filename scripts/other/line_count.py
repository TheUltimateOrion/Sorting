
#!/usr/bin/env python3

from __future__ import annotations

from pathlib import Path

WANT_EXTENSIONS: tuple[str, ...] = (".h", ".hpp", ".c", ".cpp", ".gradle", ".kt", ".txt")
EXCLUDE_DIRECTORIES: tuple[str, ...] = ("external", "build")
COLUMN_WIDTH = 40

files_read: int = 0

def get_line_count_from_file(path: Path) -> int:
    global files_read

    if not path.is_file():
        return 0

    line_count = 0
    with path.open("r", encoding="utf-8", errors="ignore") as fh:
        for _ in fh:
            line_count += 1

    files_read += 1

    return line_count


def traverse_directory(dir_path: Path) -> int:

    if not dir_path.exists():
        return 0

    dir_path = dir_path.resolve()
    
    total: int = 0

    for entry in dir_path.iterdir():
        if entry.is_file():
            if entry.suffix in WANT_EXTENSIONS:
                current_count: int = get_line_count_from_file(entry)
                total += current_count
                spaces = COLUMN_WIDTH - len(entry.name) - 1

                side_border_chars: tuple[chr, chr] = ('╟', '╢')

                top_border_char: tuple[chr, chr] = ('─', '═')

                if files_read == 1:
                    side_border_chars: tuple[chr, chr] = ('╔', '╗')
                

                print(side_border_chars[0], top_border_char[files_read == 1] * (COLUMN_WIDTH + 2), side_border_chars[1], sep='')
                print(f"║ {entry.name}:{current_count:>{spaces}} ║")
        elif entry.is_dir():
            name = entry.name
            if name.startswith("."):
                continue
            if entry.name not in EXCLUDE_DIRECTORIES:
                total += traverse_directory(entry)
    
    return total

def find_line_count() -> None:
    total_count: int =  0

    start: Path = Path.cwd().resolve()

    total_count += traverse_directory(start)
    
    print('╠' , '═' * (COLUMN_WIDTH + 2), '╣', sep='')
    print(f"║ Total: {total_count:>{COLUMN_WIDTH - 7}} ║")
    print('╚', '═' * (COLUMN_WIDTH + 2), '╝', sep='')


def main() -> None:
    find_line_count()



if __name__ == "__main__":
    main()