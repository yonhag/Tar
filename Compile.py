import os
import shutil
import time
import subprocess
import docker


# IMPORTANT:
# Swap the directories below this to the correct ones on your machine.

CLEAN_COMPILATION = False # Change to True to delete object files before compilation

relay_src = r"./Relay"
relay_dst = r"\\wsl.localhost\Ubuntu-22.04\home\yona\makefile_relay"

dir_src = r"./Directory"
dir_dst = r"\\wsl.localhost\Ubuntu-22.04\home\yona\makefile_directory"

relay_docker = './Docker/relay'
dir_docker = './Docker/dir'

relay_build_command = 'docker build -t relay .'
dir_build_command = 'docker build -t dir .'

source_file_extentions = ['.cpp', '.h', '.hpp']

delete_files_command = 'make clean'
compile_command = 'make'

def main():
    remove_existing_containers()
    option = display_menu()
    if option == '0':
        run_project()
        return
    elif option == '1':
        build_relay()
    elif option == '2':
        build_dir()
    else:
        copy_source_files()
        compile_files()
        transfer_executables()
        build_docker_images()

    print('Done!')
    
    run = input('Run all? (0 if not)\n- ')
    if run != '0':
        run_project()

def remove_existing_containers():
    print('Removing containers')
    client = docker.from_env()
    containers = client.containers.list(all=True)

    for container in containers:
        try:
            container.remove(force=True)
            print(f"Container {container.id} removed successfully.")
        except Exception as e:
            print(f"Failed to remove container {container.id}: {e}")

def run_project():
    run_relay_command = 'docker run -it -p 8203:8203 --network=tor --ip 192.168.0.3 relay 8203'
    run_dir_command = 'docker run -it -p 8201:8201 --network=tor --ip 192.168.0.27 dir'
    
    # Running dir
    subprocess.Popen(['start', 'cmd', '/k', run_dir_command], shell=True)

    # Running relay x3
    for i in range(4, 7):
        time.sleep(0.5)
        subprocess.Popen(['start', 'cmd', '/k', run_relay_command.replace('3', str(i))], shell=True)

def build_dir():
    copy_dir_source()
    compile_dir()
    transfer_dir_executable()
    build_dir_image()

def build_relay():
    copy_relay_source()
    compile_relay()
    transfer_relay_executable()
    build_relay_image()

def display_menu():
    print('Welcome to team 1102 Tor project builder!')
    print('Choose an option:')
    print('0. Run Project')
    print('1. Compile & Build Relay')
    print('2. Compile & Build Directory')
    print('3. Compile & Run Both')
    return input("- ")        

def build_docker_images():
    build_relay_image()
    build_dir_image()
    print()

def build_relay_image():
    remove_docker_image('relay')
    
    print('Building Relay')
    print(execute_cmd_command(relay_build_command, relay_docker))

def build_dir_image():
    remove_docker_image('dir')
    
    print('Building Directory')
    print(execute_cmd_command(dir_build_command, dir_docker))

def remove_docker_image(image_name):
    client = docker.from_env()

    try:
        client.images.remove(image_name)
        print(f"Image '{image_name}' removed successfully.")
    except docker.errors.ImageNotFound:
        print(f"Image '{image_name}' not found.")
    except docker.errors.APIError as e:
        print(f"Failed to remove image '{image_name}': {e}")

def execute_cmd_command(command, directory):
    try:
        result = subprocess.run(command, capture_output=True, text=True, check=True, cwd=directory)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return f"Error executing command: {e}"

def transfer_executables():
    transfer_relay_executable()
    transfer_dir_executable()
    print()

def transfer_dir_executable():
    dir_exe = "\Directory"
    print("Copying", dir_exe)
    shutil.copy2(dir_dst + dir_exe, dir_docker + dir_exe)

def transfer_relay_executable():
    relay_exe = "\Relay"
    print("Copying", relay_exe)
    shutil.copy2(relay_dst + relay_exe, relay_docker + relay_exe)

def compile_files():
    compile_relay()
    compile_dir()
    print()

def compile_dir():
    print('Compiling Directory:')
    if CLEAN_COMPILATION:
        print(execute_wsl_command(delete_files_command, dir_dst))
    print(execute_wsl_command(compile_command, dir_dst))

def compile_relay():
    print('Compiling Relay:')
    if CLEAN_COMPILATION:
        print(execute_wsl_command(delete_files_command, relay_dst))
    print(execute_wsl_command(compile_command, relay_dst))

def execute_wsl_command(command, wsl_directory):
    try:
        # Use subprocess to run the command in WSL
        result = subprocess.run(['wsl', 'bash', '-c', command], capture_output=True, text=True, check=True, cwd=wsl_directory)
        # Return the output of the command
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return f"Error executing WSL command: {e}"

def copy_source_files():
    copy_relay_source()
    copy_dir_source()
    print()

def copy_dir_source():
    global dir_src, dir_dst
    copy_file_directory(dir_src, dir_dst)
    print('Copied Directory')

def copy_relay_source():
    global relay_src, relay_dst
    copy_file_directory(relay_src, relay_dst)
    print('Copied Relay')

def copy_file_directory(source_folder, dest_folder):
    for file_name in os.listdir(source_folder):
        # construct full file path
        if is_source_file(file_name):
            source = source_folder + "/" + file_name
            destination = dest_folder + "/" + file_name
            # copy only files
            if os.path.isfile(source):
                shutil.copy2(source, destination)

def is_source_file(filename):
    for extension in source_file_extentions:
        if filename.endswith(extension):
            return True
    return False


if __name__ == '__main__':
    main()