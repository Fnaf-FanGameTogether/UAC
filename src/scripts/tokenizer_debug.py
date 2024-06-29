import sys, os

"""
this script read lines and creates temporal files
in which the lines are dumped into

so that the tokenizer reads from a file

just, please, please, don't judge us, specially manuel, I ain't reading from stdin
not in C, please no

"""

def exec(file, prompt = None):
    prompt = prompt or '>>>'
    with open(file, 'w', encoding='utf-8') as f:
        f.write(input(prompt))

def main(argc, argv):
    """
    usage:
    
        // write into file whatever was prompted
        python tok...bug.py <filename> [<prompt>]
        or
        // delete file
        python tok...bug.py -d <filename>
    
    """
    if argc < 2:
        exit(1)
    
    filename = argv[1]
    prompt = argv[2] if argc >= 3 else None
    if(filename == '-d'):
        filename = argv[2] if argc >= 3 else None
        
        if not filename:
            exit(1)
        os.remove(filename)
        exit(0)
    try:
        exec(filename, prompt)
    except KeyboardInterrupt:
        exit(0)


if __name__ == '__main__':
    main(len(sys.argv), sys.argv)

