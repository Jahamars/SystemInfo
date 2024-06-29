#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <array>
#include <sstream>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

bool commandExists(const std::string& cmd) {
    std::string check_cmd = "command -v " + cmd + " > /dev/null 2>&1";
    return (system(check_cmd.c_str()) == 0);
}

void writeSystemInfo(std::ofstream& file) {
    std::stringstream ss;

    ss << "System:\n" << exec("uname -a") << std::endl;
    ss << "Operating System:\n" << exec("cat /etc/os-release") << std::endl;
    ss << "Processor:\n" << exec("lscpu") << std::endl;
    ss << "Memory:\n" << exec("free -h") << std::endl;
    ss << "Memory Details:\n" << exec("cat /proc/meminfo") << std::endl;
    ss << "Disks:\n" << exec("lsblk") << std::endl;
    ss << "File System:\n" << exec("df -h") << std::endl;
    ss << "Mounted Filesystems:\n" << exec("mount") << std::endl;
    ss << "Network Interfaces:\n" << exec("ip a") << std::endl;
    ss << "Routing Table:\n" << exec("ip r") << std::endl;

    if (commandExists("arp")) {
        ss << "ARP Table:\n" << exec("arp -a") << std::endl;
    } else {
        ss << "ARP Table: command not found\n";
    }

    if (commandExists("wmctrl")) {
        ss << "Window Manager:\n" << exec("wmctrl -m") << std::endl;
    } else {
        ss << "Window Manager: command not found\n";
    }

    ss << "Desktop Environment:\n" << exec("echo $XDG_CURRENT_DESKTOP") << std::endl;
    ss << "Theme:\n" << exec("gsettings get org.gnome.desktop.interface gtk-theme") << std::endl;

    if (std::ifstream("/etc/X11/default-display-manager").good()) {
        ss << "Display Manager:\n" << exec("cat /etc/X11/default-display-manager") << std::endl;
    } else {
        ss << "Display Manager: file not found\n";
    }

    ss << "Desktop Background:\n" << exec("gsettings get org.gnome.desktop.background picture-uri") << std::endl;
    ss << "Taskbar and Menu:\n" << exec("gsettings get org.gnome.shell.extensions.dash-to-dock dock-fixed") << std::endl;
    ss << "Terminal Emulator:\n" << exec("echo $TERM") << std::endl;

    if (commandExists("ls")) {
        ss << "Plugins and Extensions:\n" << exec("ls ~/.local/share/gnome-shell/extensions") << std::endl;
    } else {
        ss << "Plugins and Extensions: command not found\n";
    }

    ss << "Compositing Manager:\n" << exec("ps -e | grep -i 'compton\\|picom\\|kwin\\|mutter\\|compiz'") << std::endl;
    ss << "Lock Screen Background:\n" << exec("gsettings get org.gnome.desktop.screensaver picture-uri") << std::endl;
    ss << "Font Settings:\n" << exec("fc-match") << std::endl;

    if (commandExists("dpkg-query")) {
        ss << "Installed Packages:\n" << exec("dpkg-query -l") << std::endl;
    } else {
        ss << "Installed Packages: command not found\n";
    }

    ss << "Processor Details:\n" << exec("cat /proc/cpuinfo") << std::endl;
    ss << "System Uptime:\n" << exec("uptime") << std::endl;
    ss << "Running Processes:\n" << exec("ps aux") << std::endl;
    ss << "Loaded Kernel Modules:\n" << exec("lsmod") << std::endl;
    ss << "PCI Devices:\n" << exec("lspci") << std::endl;

    if (commandExists("lsusb")) {
        ss << "USB Devices:\n" << exec("lsusb") << std::endl;
    } else {
        ss << "USB Devices: command not found\n";
    }

    ss << "Temperature Sensors:\n" << exec("sensors") << std::endl;
    ss << "Graphics Card Info:\n" << exec("lspci | grep -i vga") << std::endl;
    ss << "Environment Variables:\n" << exec("printenv") << std::endl;

    if (commandExists("journalctl")) {
        ss << "System Logs:\n" << exec("journalctl -n 100") << std::endl; // Last 100 lines of system log
    } else {
        ss << "System Logs: command not found\n";
    }

    ss << "Kernel Version:\n" << exec("uname -r") << std::endl;

    if (commandExists("hostname")) {
        ss << "Hostname:\n" << exec("hostname") << std::endl;
    } else {
        ss << "Hostname: command not found\n";
    }

    ss << "User Information:\n" << exec("whoami") << std::endl;
    ss << "Groups:\n" << exec("groups") << std::endl;
    ss << "Available Shells:\n" << exec("cat /etc/shells") << std::endl;

    if (commandExists("systemctl")) {
        ss << "Services (systemctl):\n" << exec("systemctl list-units --type=service --state=running") << std::endl;
    } else {
        ss << "Services (systemctl): command not found\n";
    }

    if (commandExists("sudo")) {
        ss << "Firewall Status:\n" << exec("sudo ufw status") << std::endl;
    } else {
        ss << "Firewall Status: sudo command not found or requires password\n";
    }

    if (commandExists("crontab")) {
        ss << "Cron Jobs:\n" << exec("crontab -l") << std::endl;
    } else {
        ss << "Cron Jobs: command not found\n";
    }

    if (commandExists("atq")) {
        ss << "Scheduled Tasks:\n" << exec("atq") << std::endl;
    } else {
        ss << "Scheduled Tasks: command not found\n";
    }

    ss << "Swap Usage:\n" << exec("swapon --show") << std::endl;
    ss << "Block Devices:\n" << exec("lsblk") << std::endl;

    if (commandExists("iostat")) {
        ss << "I/O Statistics:\n" << exec("iostat") << std::endl;
    } else {
        ss << "I/O Statistics: command not found\n";
    }

    ss << "Top 10 Memory Consuming Processes:\n" << exec("ps aux --sort=-%mem | head -n 11") << std::endl;
    ss << "Top 10 CPU Consuming Processes:\n" << exec("ps aux --sort=-%cpu | head -n 11") << std::endl;
    ss << "Listening Ports:\n" << exec("ss -tuln") << std::endl;
    ss << "Open Files:\n" << exec("lsof") << std::endl;

    if (commandExists("sestatus")) {
        ss << "SELinux Status:\n" << exec("sestatus") << std::endl;
    } else {
        ss << "SELinux Status: command not found\n";
    }

    if (commandExists("docker")) {
        ss << "Docker Containers:\n" << exec("docker ps") << std::endl;
    } else {
        ss << "Docker Containers: command not found\n";
    }

    if (commandExists("virsh")) {
        ss << "Virtual Machines (KVM):\n" << exec("virsh list") << std::endl;
    } else {
        ss << "Virtual Machines (KVM): command not found\n";
    }

    if (commandExists("lshw")) {
        ss << "Hardware Info:\n" << exec("lshw") << std::endl;
    } else {
        ss << "Hardware Info: command not found\n";
    }

    if (commandExists("dmesg")) {
        ss << "DMESG Log (last 100 lines):\n" << exec("dmesg | tail -n 100") << std::endl;
    } else {
        ss << "DMESG Log: command not found or permission denied\n";
    }

    file << ss.str();
}

int main() {
    std::string filename = "system_info.txt";
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }
        writeSystemInfo(file);
        std::cout << "System information has been written to " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
