#include "AgendaUI.hpp"
#include <iomanip>
    // std::string m_userName;
    // std::string m_userPassword;
    // AgendaService m_agendaService;

AgendaUI::AgendaUI() {
    m_userName = m_userPassword = "";
}

void AgendaUI::OperationLoop(void) {
    bool flag = true;
    while (flag) {
        startAgenda();
        std::string command = getOperation();
        if (command == "l") {
            userLogIn();
        } else if (command == "r") {
            userRegister();
        } else if (command == "q") {
            quitAgenda();
            flag = false;
        } else if (command == "f") {
            findPassword();
        } else {
            std::cout << "Invalid command!\n";
        }
    }
}

//   private:

void AgendaUI::startAgenda(void) {
    std::cout << "\n-------------------- Agenda ------------------------\n"
    << "Action :\n"
    << "l    - log in Agenda by username\n"
    << "r    - register an Agenda account\n"
    << "f    - find back password\n"
    << "q    - quit Agenda\n"
    << "----------------------------------------------------\n\n\n"
    << "Agenda : ~$ ";
}

std::string AgendaUI::getOperation() {
    std::string oper;
    std::cin >> oper;
    return oper;
}

    /**
     * execute the operation
     * @return if the operationloop continue
     */
bool AgendaUI::executeOperation(std::string t_operation) {
    bool flag = true;
    while (flag) {
        std::cout << "\n-------------------- Agenda -----------------------\n"
        << "Action :\n"
        << "o   - log out Agenda\n"
        << "dc  - delete Agenda account\n"
        << "lu  - list all Agenda user\n"
        << "su  - search a user by name\n"
        << "cm  - create a meeting\n"
        << "la  - list all meetings\n"
        << "las - list all sponsor meetings\n"
        << "lap - list all participate meetings\n"
        << "qm  - query meeting by title\n"
        << "qt  - query meeting by time interval\n"
        << "dm  - delete meeting by title\n"
        << "da  - delete all meetings\n"
        << "rp  - reset password\n"
        << "sf  - save data immediately\n"
        << "----------------------------------------------------\n\n\n"
        << "Agenda@" << m_userName << " : # ";
        std::string command = getOperation(); 
        if (command == "o") {
            userLogOut();
            flag = false;
        } else if (command == "dc") {
            deleteUser();
            flag = false;
            m_userName = m_userPassword = "";
        } else if (command == "lu") {
            listAllUsers();
        } else if (command == "su") {
            searchUser();
        } else if (command == "cm") {
            createMeeting();
        } else if (command == "la") {
            listAllMeetings();
        } else if (command == "las") {
            listAllSponsorMeetings();
        } else if (command == "lap") {
            listAllParticipateMeetings();
        } else if (command == "qm") {
            queryMeetingByTitle();
        } else if (command == "qt") {
            queryMeetingByTimeInterval();
        } else if (command == "dm") {
            deleteMeetingByTitle();
        } else if (command == "da") {
            deleteAllMeetings();
        } else if (command == "rp") {
            resetPassword();
        } else if (command == "sf") {
            saveinfile();
        } else {
            std::cout << "Invalid command!\n";
        }
    }
    return true;
}

    /**
     * user Login
     */
void AgendaUI::userLogIn(void) {
    std::cout << "[log in] [user name] [password]\n";
    std::cout << "[log in] ";
    std::string username, password;
    std::cin >> username >> password;
    if (m_agendaService.userLogIn(username,password)) {
        m_userName = username;
        m_userPassword = password;
        std::cout << "[log in] succeed!\n\n";
        std::string com = "nothing";
        executeOperation(com);
    } else {
        std::cout << "[error] log in fail!\n";
        std::cout << "--------------------\nForget your password?\n"
        << "Try to input \"f\" to find back your password\n"
        << "--------------------\n";
    }
}

    /**
     * user regist
     */
void AgendaUI::userRegister(void) {
    std::cout << "[register] [username] [password] [email] [phone]\n"
    << "[register] ";
    std::string username,password,email,phone;
    std::cin >> username >> password >> email >> phone;
    if (m_agendaService.userRegister(username,password,email,phone))
        std::cout << "[register] succeed!\n";
    else
        std::cout << "[error] register fail!\n";
}

void AgendaUI::quitAgenda(void) {
    std::cout << "\n----------------------------------------------------"
    << "\nThanks for using Agenda~\n"
    << "----------------------------------------------------\n\n";
}

void AgendaUI::userLogOut(void) {
    m_userName = m_userPassword = "";
}

void AgendaUI::deleteUser(void) {
    if (m_agendaService.deleteUser(m_userName,m_userPassword))
        std::cout << "[delete agenda account] succeed!\n";
}


void AgendaUI::listAllUsers(void) {
    std::cout << "[list all users]\n"
    << "name     " << "email            " << "phone      \n"; 
    std::list<User> li = m_agendaService.listAllUsers();
    for (auto u : li) {
        std::cout << std::left << std::setw(9) << u.getName()
        << std::setw(17) << u.getEmail()
        << std::setw(11) << u.getPhone() << std::endl;
    }
}

void AgendaUI::createMeeting(void) {
    std::cout << "[create meeting] [the number of participate]\n"
    << "[create meeting] ";
    int number;
    std::cin >> number;
    std::vector<std::string> vec;
    for (int i = 1; i <= number; i++) {
        std::cout << "[create meeting] [please enter the participator "
        << i << " ]\n" << "[create meeting] ";
        std::string temp;
        std::cin >> temp;
        vec.push_back(temp);
    }
    std::cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
    << "\n[create meeting] ";
    std::string title,stime,etime;
    std::cin >> title >> stime >> etime;
    std::cout << "[create meeting] ";
    if (m_agendaService.createMeeting(m_userName,title,stime,etime,vec))
        std::cout << "succeed!\n";
    // else
    //     std::cout << "error!\n";
}

void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]\n\n";
    std::list<Meeting> li = m_agendaService.listAllMeetings(m_userName);
    printMeetings(li);
}

void AgendaUI::listAllSponsorMeetings(void) {
    std::cout << "[list all sponsor meetings]\n\n";
    std::list<Meeting> li = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(li);
}

void AgendaUI::listAllParticipateMeetings(void) {
    std::cout << "[list all participator meetings]\n\n";
    std::list<Meeting> li = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(li);
}

void AgendaUI::queryMeetingByTitle(void) {
    std::cout << "[query meeting] [title]:\n" << "[query meeting] ";
    std::string title;
    std::cin >> title;
    std::list<Meeting> li = m_agendaService.meetingQuery(m_userName,title);
    printMeetings(li);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
    << "[query meetings] ";
    std::string stime,etime;
    std::cin >> stime >> etime;
    std::list<Meeting> li = m_agendaService.meetingQuery(m_userName,stime,etime);
    std::cout << std::endl;
    printMeetings(li);
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::cout << "[delete meeting] [title]\n[delete meeting] ";
    std::string title;
    std::cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title))
            std::cout << "\n[delete meeting by title] succeed!\n";
    else
        std::cout << "\n[error] delete meeting fail!\n";
}

void AgendaUI::deleteAllMeetings(void) {
    m_agendaService.deleteAllMeetings(m_userName);
    std::cout << "\n[delete all meetings] succeed!\n";
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    std::cout << "title          " << "sponsor   "
    << "start time        " << "end time          " << "participators\n";
    for (auto me : t_meetings) {
        std::cout << std::left << std::setw(15) << me.getTitle()
        << std::setw(10) << me.getSponsor()
        << std::setw(18) << Date::dateToString(me.getStartDate())
        << std::setw(18) << Date::dateToString(me.getEndDate());
        std::vector<std::string> vec = me.getParticipator();
        int size = vec.size();
        auto iter = vec.begin();
        for (int i = 0; i < size - 1; i++) {
            std::cout << (*iter) << ',';
            ++iter;     
        }
        std::cout << vec.back() << std::endl;
    }
}

void AgendaUI::resetPassword(void) {
    std::cout << "[reset password] [Please enter your new password]\n"
    << "[reset password] ";
    std::string new_password, con_password;
    std::cin >> new_password;
    std::cout << "[reset password] [Please confirm your new password]\n"
    << "[reset password] ";
    std::cin >> con_password;
    if (new_password == con_password && m_agendaService.resetPassword(m_userName,new_password))
        std::cout << "[reset password] succeed!\n";
    else
        std::cout << "[reset password] error\n";    
}

void AgendaUI::findPassword(void) {
    std::cout << "\n[find back password] [username] [email] [phone]\n"
    << "[find back password] ";
    std::string username, email, phone, password;
    std::cin >> username >> email >> phone;
    password = m_agendaService.findPassword(username,email,phone);
    if (password == "")
        std::cout << "[find back password] Information error!\n";
    else
        std::cout << "[find back password] succeed!\n"
        << "[find back password] " << "Your password is [" << password << "]\n";
}

void AgendaUI::searchUser(void) {
    std::cout << "\n[search user] [username]\n"
    << "[search user] ";
    std::string username;
    std::cin >> username;
    User ans = m_agendaService.searchUser(username);
    if (ans.getName() != "") {
        std::cout << "[search user] succeed!\n"
        << "[username]     [email]          [phone]\n"
        << std::left << std::setw(15) << ans.getName()
        << std::setw(17) << ans.getEmail() << ans.getPhone() << std::endl;
    } else {
        std::cout << "[search user] user [" << username
            << "] does not exist!\n";
    }
}

void AgendaUI::saveinfile(void) {
    if (m_agendaService.saveinfile())
        std::cout << "\ndata saved!\n";
    else
        std::cout << "\nerror!\n";
}