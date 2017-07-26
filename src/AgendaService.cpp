#include "AgendaService.hpp"
#include <iostream>
//std::shared_ptr<Storage> m_storage;

bool searchall(Date s1, Date e1, Date s2, Date e2) {
    if ((s1 < e1 && e1 < s2 && s2 < e2) ||
        (s2 < e2 && e2 < s1 && s1 < e1))
        return false;
    else
        return true;
}

bool validcreate(Date s1, Date e1, Date sx, Date ex) {
    if ((s1 < e1 && e1 <= sx && sx < ex) ||
        (sx < ex && ex <= s1 && s1 < e1))
        return true;
    else
        return false;
}


AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

bool AgendaService::userLogIn(const std::string userName, const std::string password) {
    for (User& u : listAllUsers()) {
        if (u.getName() == userName) {
            if (u.getPassword() == password)
                return true;
            else {
                std::cout << "[error] wrong password!\n";
                return false;
            }
        }
    }
    std::cout << "user [" << userName << "] does not exist!\n";
    return false;
}

bool AgendaService::userRegister(const std::string userName, const std::string password,
                      const std::string email, const std::string phone) {
    for (User& u : m_storage->queryUser([](const User&)->bool {
        return true;
    }))
        if (u.getName() == userName) {
            std::cout << "[error] user name [" << userName << "] has been registered!\n";
            return false;
        }
    User one(userName, password, email, phone);
    m_storage -> createUser(one);
    return true;
}

bool AgendaService::deleteUser(const std::string userName, const std::string password) {
    if (m_storage -> deleteUser([userName, password](const User & u)
        -> bool {
            if (u.getName() == userName && u.getPassword() == password)
                return true;
            else
                return false;
        }) > 0) {
        deleteAllMeetings(userName);
        m_storage -> deleteMeeting([userName, password](const Meeting & me)
        -> bool {
            if (me.isParticipator(userName))
                return true;
            else
                return false;
        });
        return true;
    } else {
        return false;      
    }

}

std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage -> queryUser([](const User & u) -> bool {
        return true;
    });
}

bool AgendaService::createMeeting(const std::string userName, const std::string title,
                    const std::string startDate, const std::string endDate,
                    const std::vector<std::string> participator) {
    // judge Date
    if (endDate <= startDate) {
        std::cout << "Invalid date input!\n";
        return false;    
    }

    if (participator.empty()) {
        std::cout << "There should be at least one participator\n";
        return false;
    }

    for (auto iter = participator.begin(); iter != participator.end() - 1; iter++)
        for (auto it = iter+1; it != participator.end(); it++)
            if ((*iter) == (*it)) {
                std::cout << "You should not add the same participator to the list more than once\n";
                return false;
            }
    for (auto me : listAllMeetings(userName)) {
        if (!validcreate(Date::stringToDate(startDate),Date::stringToDate(endDate),me.getStartDate(),me.getEndDate())) {
            std::cout << "The date is overlap with your meeting [" << me.getTitle() << "]\n";
            return false; 
        }
    }

    // judge sponsor
    bool have_user = false;
    for (auto u : listAllUsers())
        if (u.getName() == userName)
            have_user = true;
    if (have_user == false) {
        std::cout << "You are not in the list of users\n";
        return false;   
    }


    // judge participator
    for (auto p : participator) {
        
        bool have_p = false;
        for (auto u : listAllUsers())
            if (u.getName() == p)
                have_p = true;

        if (have_p == false) {
            std::cout << "The participator [" << p << "] is not in the list of users\n";
            return false;  
        }


        for (auto me : listAllMeetings(p)) {
            if (!validcreate(Date::stringToDate(startDate),Date::stringToDate(endDate),me.getStartDate(),me.getEndDate())) {
                std::cout << "The date is overlaped with [" << p << "]'s meeting [" << me.getTitle() << "]\n";
                return false;               
            }

        }
    
    }

    // judge repeat
    for (auto p : participator)
        if (p == userName) {
            std::cout << "You should not add yourself to the list of participators\n";
            return false;    
        }


    //judge title
    for (auto me :listAllMeetings(userName)) {
        if (me.getTitle() == title) {
            std::cout << "The title has been used\n";
            return false;      
        }

    }
    Meeting one(userName,participator,Date::stringToDate(startDate),Date::stringToDate(endDate),title);
    m_storage -> createMeeting(one);
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string title) const {
    return m_storage -> queryMeeting([userName,title](const Meeting & me)
        -> bool {
            if ((me.getSponsor() == userName && me.getTitle() == title) ||
                (me.isParticipator(userName) && me.getTitle() == title))
                return true;
            else
                return false;
        }
        );
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string startDate,
                                    const std::string endDate) const {
    if (startDate > endDate) {
        std::list<Meeting> li;
        return li;
    }
    if (startDate == endDate) {
            return m_storage -> queryMeeting([userName,startDate,endDate](const Meeting & me)
        -> bool {
            if (me.getStartDate() <= Date::stringToDate(startDate) &&
                me.getEndDate() >= Date::stringToDate(startDate))
                return true;
            else
                return false;
        }
        );
    }
    if ((!Date::isValid(Date::stringToDate(startDate))) || (!Date::isValid(Date::stringToDate(endDate)))) {
        std::list<Meeting> li;
        return li;
    }

    return m_storage -> queryMeeting([userName,startDate,endDate](const Meeting & me)
        -> bool {
            bool flag_1 = false;
            bool flag_2 = false;
            if (me.getSponsor() == userName || me.isParticipator(userName))
                flag_1 = true;
            if (searchall(Date::stringToDate(startDate),Date::stringToDate(endDate),me.getStartDate(),me.getEndDate()))
                flag_2 = true;
            return flag_1 && flag_2;
        }
        );   
}

    /**
     * list all meetings the user take part in
     * @param userName user's username
     * @return a meeting list result
     */
std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
    return m_storage -> queryMeeting([userName](const Meeting & me)
        -> bool {
            if (me.isParticipator(userName) || me.getSponsor() == userName)
                return true;
            else
                return false;
        }
        );
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
    return m_storage -> queryMeeting([userName](const Meeting & me)
        -> bool {
            if (me.getSponsor() == userName)
                return true;
            else
                return false;
        }
        );
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string userName) const {
    return m_storage -> queryMeeting([userName](const Meeting & me)
        -> bool {
            if (me.isParticipator(userName))
                return true;
            else
                return false;
        }
        );
}

    /**
     * delete a meeting by title and its sponsor
     * @param userName sponsor's username
     * @param title meeting's title
     * @return if success, true will be returned
     */
bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
    if (m_storage -> deleteMeeting([userName,title](const Meeting & me)
        -> bool {
            if (me.getSponsor() == userName && me.getTitle() == title)
                return true;
            else
                return false;
        }
        ) > 0)
        return true;
    else
        return false;
}

    /**
     * delete all meetings by sponsor
     * @param userName sponsor's username
     * @return if success, true will be returned
     */
bool AgendaService::deleteAllMeetings(const std::string userName) {
    if (m_storage -> deleteMeeting([userName](const Meeting & me)
        -> bool {
            if (me.getSponsor() == userName)
                return true;
            else
                return false;
        }
        ) >= 0)
        return true;
    else
        return false;
}

    /**
     * start Agenda service and connect to storage
     */
void AgendaService::startAgenda(void) {
    m_storage = m_storage -> getInstance();
}

    /**
     * quit Agenda service
     */
void AgendaService::quitAgenda(void) {
    m_storage -> sync();
}

// extra

bool AgendaService::resetPassword(const std::string username, const std::string password) {
    if (m_storage->updateUser([username,password](const User & u) -> bool {
        if (u.getName() == username)
            return true;
        else
            return false;
    }, [username,password](User & u) -> void {
        u.setPassword(password);
    }) > 0)
        return true;
    else
        return false;
}

std::string AgendaService::findPassword(const std::string username, const std::string email, const std::string phone) {
    std::list<User> li = m_storage->queryUser([username,email,phone](const User & u) -> bool {
        if (u.getName() == username && u.getEmail() == email && u.getPhone() == phone)
            return true;
        else
            return false;
    });
    if (li.empty()) {
        std::string str = "";
        return str;
    } else {
        return li.begin()->getPassword();      
    }

}

User AgendaService::searchUser(const std::string username) {
    std::list<User> li = m_storage->queryUser([username](const User & u) -> bool {
        if (u.getName() == username)
            return true;
        else
            return false;
    });
    if (li.empty()) {
        User ans("","","","");
        return ans;
    } else {
        return li.front();
    }
}
bool AgendaService::saveinfile(void) {
    return m_storage->sync();
}