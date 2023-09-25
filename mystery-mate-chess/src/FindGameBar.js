function FindGameBar() {
    const [gameIdToJoin, setGameIdToJoin] = useState("");
    
    const joinGame = async (horcruxeId) => {
        try {
            const response = await fetch(apiBaseUrl + '/startNew', {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify(horcruxeId),
            });

            if (!response.ok) throw new Error('Network response was not ok');
            const data = await response.json();
            console.log('Joined game:', data);

        } catch (error) {
            console.error('There was a problem with the fetch operation:', error);
        }
    }
    
    return (
      <div>
        <input value={gameIdToJoin} onChange={(e) => setGameIdToJoin(e.target.value)} placeholder="Game ID" />
        <button onClick={joinGame}>Join</button>
      </div>
    );
  }