#include <iostream>
#include <set>
#include <unordered_map>
#include <string>
#include <algorithm>

typedef int track_type;
typedef int score_type;

struct Track {
	track_type id;
	score_type score;

	Track(track_type idT = 0, score_type scoreT = 0) {
		id = idT;
		score = scoreT;
	}
};

struct Vote {
	std::string ip;
	track_type track;
	score_type score;
	size_t time;
};

class Radio {

	class Track_Rater {
	public:
		bool operator()(const Track& a, const Track& b) const {
			if (a.score != b.score) {
				return a.score > b.score;
			}
			else
				return a.id < b.id;
		}
	};

	std::unordered_map<std::string, size_t> last_votations;
	std::unordered_map<track_type, score_type> scores;
	std::set<Track, Track_Rater> rating;
	track_type first_unexist_track = 1;

	void insert(track_type id, score_type score) {
		rating.emplace(id, score);
		scores.emplace(id, score);
	}

	void erase(const Track& track) {
		rating.erase(track);
		scores.erase(track.id);
	}

	void find_FUT() {
		while (scores.find(first_unexist_track) != scores.end()) {
			++first_unexist_track;
		}
	}

public:
	Track Get() {
		if (rating.size() == 0) {
			insert(1, -1);
			return Track(1, 0);
		}

		Track candidate_to_get = *rating.begin();

		if (candidate_to_get.score > 0) {
			erase(candidate_to_get);
			insert(candidate_to_get.id, -1);
			return Track(candidate_to_get.id, candidate_to_get.score);
		}

		find_FUT();

		if (candidate_to_get.score == 0) {
			track_type id = std::min(candidate_to_get.id, first_unexist_track);
			erase(Track(id, 0));
			insert(id, -1);
			return Track(id, 0);
		}

		insert(first_unexist_track, -1);
		return Track(first_unexist_track, 0);
	}

	Track Vote(const Vote& vote) {
		score_type score = scores[vote.track];

		erase(Track(vote.track, score));

		if (last_votations.find(vote.ip) == last_votations.end() || vote.time - last_votations[vote.ip] >= 600) {
			score += vote.score;
			last_votations[vote.ip] = vote.time;
		}

		insert(vote.track, score);

		return Track(vote.track, score);
	}
};

int main() {

	Radio radio;
	std::string command;
	std::cin >> command;

	Vote vote;
	Track track;
	std::string stop("EXIT");


	while (command != stop) {
		if (command == "GET") {
			track = radio.Get();
			std::cout << track.id << ' ' << track.score << '\n';
		}

		if (command == "VOTE") {
			std::cin >> vote.ip >> vote.track >> vote.score >> vote.time;
			track = radio.Vote(vote);
			std::cout << track.score << '\n';
		}

		std::cin >> command;
	}

	std::cout << "OK\n";

	return 0;
}