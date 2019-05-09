function getShortMessages(messages) {
    return messages.filter(obj => obj.message.length < 50).map(obj => obj.message)
}

module.exports = getShortMessages