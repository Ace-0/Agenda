#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <sstream>

  // static std::shared_ptr<Storage> m_instance;
  // std::list<User> m_userList;
  // std::list<Meeting> m_meetingList;
  // bool m_dirty;
std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
  readFromFile();
  m_dirty = false;
}

bool Storage::readFromFile(void) {
  std::fstream u_fin(Path::userPath,std::ios::in);
  std::string u_line;
  std::fstream me_fin(Path::meetingPath,std::ios::in);
  std::string me_line;

  if (!(u_fin.is_open() && me_fin.is_open())) {
    return false;
  }
  while (getline(u_fin, u_line)) {
    std::istringstream u_sin(u_line);
    std::string u_word;
    std::vector<std::string> u_vec;
    while (getline(u_sin, u_word, ',')) {
      u_vec.push_back(u_word); 
    }
    User t_user;
    t_user.setName(u_vec[0].substr(1, u_vec[0].size() - 2));
    t_user.setPassword(u_vec[1].substr(1, u_vec[1].size() - 2));
    t_user.setEmail(u_vec[2].substr(1, u_vec[2].size() - 2));
    t_user.setPhone(u_vec[3].substr(1, u_vec[3].size() - 2));
    m_userList.push_back(t_user);
  }

  // every line
  while (getline(me_fin, me_line)) {
    std::istringstream me_sin(me_line);
    std::string me_word;
    std::vector<std::string> me_vec;
    while (getline(me_sin, me_word, ',')) {
      me_vec.push_back(me_word); 
    }
    Meeting t_meeting;
    t_meeting.setSponsor(me_vec[0].substr(1, me_vec[0].size() - 2));
    std::vector<std::string> par_vec;
    int start = 1, end = me_vec[1].size() - 1;
    for (int i = 1; i < me_vec[1].size(); i++) {
      if (me_vec[1][i] == '&') {
        end = i;
        std::string one = me_vec[1].substr(start, end - start);
        start = i + 1;
        par_vec.push_back(one);
      }
    }
    end = me_vec[1].size() - 1;
    std::string one = me_vec[1].substr(start, end - start);
    par_vec.push_back(one);
    t_meeting.setParticipator(par_vec);
    t_meeting.setStartDate(Date::stringToDate(me_vec[2].substr(1, me_vec[2].size() - 2)));
    t_meeting.setEndDate(Date::stringToDate(me_vec[3].substr(1, me_vec[3].size() - 2)));
    t_meeting.setTitle(me_vec[4].substr(1, me_vec[4].size() - 2));
    m_meetingList.push_back(t_meeting);
  }
  return true;
}

bool Storage::writeToFile(void) {
  std::fstream u_fout(Path::userPath,std::ios::out);
  std::fstream me_fout(Path::meetingPath,std::ios::out);

  if (!(u_fout.is_open() && me_fout.is_open())) {
    return false;
  }
  
//  u_fout << "\"name\"," << "\"password\"," << "\"email\"," << "\"phone\"" << std::endl;
  for (auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
    u_fout << "\"" << (*iter).getName() << "\",\"" << (*iter).getPassword() << "\",\""
    << (*iter).getEmail() << "\",\"" << (*iter).getPhone() << "\"" << std::endl;
  }

//  me_fout << "\"sponsor\"," << "\"participator\"," << "\"sdate\"," << "\"edate\"," << "\"title\"," << std::endl;
  for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
    me_fout << "\"" << (*iter).getSponsor() << "\",\"";
    std::vector<std::string> vec = (*iter).getParticipator();
    bool flag = true;
    for (auto i = vec.begin(); i != vec.end(); i++) {
      if (flag) {
        me_fout << (*i);
        flag = false;
      } else {
        me_fout << "&";
        me_fout << (*i);
      }
    }
    me_fout << "\",\"" << Date::dateToString((*iter).getStartDate());
    me_fout << "\",\"" << Date::dateToString((*iter).getEndDate());
    me_fout << "\",\"" << (*iter).getTitle() << "\"" << std::endl;
  }
  return true;
}

std::shared_ptr<Storage> Storage::getInstance(void) {
  	if (m_instance == nullptr) {
      m_instance = std::shared_ptr<Storage>(new Storage());
    }
    return m_instance; 
}

Storage::~Storage() {
  if (m_dirty == true)
    sync();
}

void Storage::createUser(const User & t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
  std::list<User> ans;
  for (auto & i : m_userList) {
    if (filter(i))
      ans.push_back(i);
  }
  return ans;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
  int counter = 0;
  for (auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
    if (filter(*iter)) {
      switcher(*iter);
      counter++;
      m_dirty = true;
    }
  }
  return counter;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
  int counter = 0;
  for (auto iter = m_userList.begin(); iter != m_userList.end(); ) {
    if (filter(*iter)) {
      counter++;
      iter = m_userList.erase(iter);
      m_dirty = true;
    } else {
      iter++;
    }
  }
  return counter;
}

void Storage::createMeeting(const Meeting & t_meeting) {
  m_meetingList.push_back(t_meeting);
  m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
  std::list<Meeting> ans;
  for (auto & i : m_meetingList) {
    if (filter(i))
      ans.push_back(i);
  }
  return ans;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
    int counter = 0;
    for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
    if (filter(*iter)) {
        counter++;
        switcher(*iter);
        m_dirty = true;
      }
    }
    return counter;
  }

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
  int counter = 0;
  for (auto iter = m_meetingList.begin(); iter != m_meetingList.end(); ) {
    if (filter(*iter)) {
      counter++;
      iter = m_meetingList.erase(iter);
      m_dirty = true;
    } else {
      iter++;
    }
  }
  return counter;
}

  /**
  * sync with the file
  */
bool Storage::sync(void) {
    return writeToFile();
  }